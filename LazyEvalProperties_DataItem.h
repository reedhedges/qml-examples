#pragma once

#include <string>

#include <QObject>
#include <QThread> // for sleep()
#include <QDebug>

#include "LazyEvalProperties_DataSource.h"

// Define to only do expensive calculation on first read, or checking for visibility
// Omit both definitions to perform the calculation on every DataItem each time the source data changes.
//#define CALC_ON_READ 1
//#define CALC_ON_VISIBLE 1 // Note if using this mode, QML component must set dataItemVisible property on component creation and when 'visible' property changes.




// How long to artificially wait in do_calculation():
static const unsigned long calculation_time_ms = 50;

static inline int do_calculation(int data)
{
  QThread::msleep(calculation_time_ms);
  return data + 100;
}


class DataItem : public QObject {
  Q_OBJECT
  // note "data", "name", etc. properties might already exist on many QML components, use more unique names
  Q_PROPERTY(QString dataItemName MEMBER name CONSTANT)
  Q_PROPERTY(int dataItemData READ getData NOTIFY notifyDataUpdated)
  Q_PROPERTY(bool isDataItemDataSet READ isDataSet NOTIFY notifyDataUpdated)
  Q_PROPERTY(bool dataItemVisible WRITE ui_visibility_changed)
private:
  QString name;
  DataSource *data_source;
  int derived_data;
  int data_updated_counter {0};
  bool first_update {true};
  bool is_visible {false};

  // We expect approximately 10 items to be initially used in the UI, so on the very first calculation, we will do this during
  // object construction.  This delays object construction however.
  static const size_t num_to_precalc = 10;
  static size_t initial_precalc_counter;

  void setup()
  {
#ifdef CALC_ON_READ
      if(initial_precalc_counter++ < num_to_precalc)
      {
          qDebug("%s.DataItem() constructor: Precalculating initial value #%lu (max %lu)", qPrintable(name), initial_precalc_counter, num_to_precalc);
          calculate_data();
      }
#endif
      connect(data_source, &DataSource::notifyDataUpdated, this, &DataItem::dataSourceUpdated);
  }

  void calculate_data()
  {
      qDebug() << "\t recalculating derived data...";
      derived_data = do_calculation(data_source->getData());
      qDebug() << "\t our derived data is now" << derived_data;
      data_updated_counter = data_source->getUpdateCounter();
      first_update = false;
  }

  void ui_visibility_changed(bool v)
  {
#ifdef CALC_ON_VISIBLE
      is_visible = v;
      if(is_visible && data_updated_counter != data_source->getUpdateCounter()) {
          calculate_data();
          emit notifyDataUpdated();
      }
#endif
  }

public:
  explicit DataItem(const std::string& _name, DataSource *_data_source) : name(QString::fromStdString(_name)), data_source(_data_source)
  {
      setup();
  }

  bool isDataSet() { return !first_update; }

#ifdef CALC_ON_READ
  int getData()
  {
    qDebug() << name << ".getData(): our counter is" << data_updated_counter << ", source counter is" << data_source->getUpdateCounter() << ", first_update? " << first_update << " source data is " << data_source->getData();
    // If source's data has changed (check by checking its counter), recalculate our derived data.
    if(first_update || (data_updated_counter != data_source->getUpdateCounter()))
    {
      calculate_data();
    }
    else
    {
      qDebug() << "\t returning cached derived data:" << derived_data;
    }
    return derived_data;
  }
#else
  int getData()
  {
      return derived_data;
  }
#endif


  virtual ~DataItem() {}
   
signals:
  void notifyDataUpdated();

public slots:
#ifdef CALC_ON_READ
  // Signal received from DataSource.  Instead of recalculating our derived data here,
  // we relay the signal to any listeners, which can call getData() to get the calculated 
  // or cached derived data. (IF no listener calls getData() data will not be calculated.)
  void dataSourceUpdated([[maybe_unused]] int source_data) {
      emit notifyDataUpdated();
  }
#else
  // Signal received from DataSource, calculate our "derived" data.
  void dataSourceUpdated(int source_data) {
    qDebug() << name << ".dataSourceUpdated() signal received: source data is " << source_data << ", is_visible? " << is_visible;
#ifdef CALC_ON_VISIBLE
    if(is_visible && data_updated_counter != data_source->getUpdateCounter())
#endif
        calculate_data();
    //qDebug() << "\tour derived data is now" << derived_data << ". Emitting update signal...";
    emit notifyDataUpdated();
  }
#endif
};




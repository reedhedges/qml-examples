#pragma once

#include <string>

#include <QObject>
#include <QThread> // for sleep()
#include <QDebug>

#include "LazyEvalProperties_DataSource.h"

// Define LAZY to only do expensive calculation on first read.
// Omit this definition to perform the calculation on every DataItem each time the source data changes.
// Note:  reads are still done by QML and long calculations may slow down QML GUI performance.
#define LAZY 1



// How long to artificially wait in do_calculation():
static const unsigned long calculation_time_ms = 200;

static inline int do_calculation(int data)
{
  QThread::msleep(calculation_time_ms);
  return data + 100;
}


class DataItem : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString name MEMBER name CONSTANT)
  Q_PROPERTY(int exampleData READ getData NOTIFY notifyDataUpdated)
  Q_PROPERTY(bool exampleDataSet MEMBER derived_data_set)
private:
  QString name;
  DataSource *data_source;
  int derived_data;
  int data_updated_counter {0};
  bool derived_data_set {false};

  void setup()
  {
     connect(data_source, &DataSource::notifyDataUpdated, this, &DataItem::dataSourceUpdated);
  }

public:
  explicit DataItem(const std::string& _name, DataSource *_data_source) : name(QString::fromStdString(_name)), data_source(_data_source)
  {
      setup();
  }

  DataItem(const DataItem& other) :
      name(other.name),
      data_source(other.data_source),
      derived_data(other.derived_data),
      data_updated_counter(other.data_updated_counter)
  {
    setup();
  }
  DataItem(DataItem&& old) noexcept :
      name(std::move(old.name)),
      data_source(old.data_source),
      derived_data(old.derived_data),
      data_updated_counter(old.data_updated_counter)
  {
      setup();
      // todo disconnect old signal
  }
  DataItem& operator=(const DataItem& other)
  {
      if(&other != this) {
          name = other.name;
          data_source = other.data_source;
          derived_data = other.derived_data;
          data_updated_counter = other.data_updated_counter;
          setup();
      }
      return *this;
  }
  DataItem& operator=(DataItem&& other) noexcept
  {
      if(&other != this) {
          name = std::move(other.name);
          data_source = other.data_source;
          derived_data = other.derived_data;
          data_updated_counter = other.data_updated_counter;
          setup();
          // todo disconnect old signal
      }
      return *this;
  }

#ifdef LAZY
  int getData()
  {
    qDebug() << name << ".getData(): our counter is" << data_updated_counter << ", source counter is" << data_source->getUpdateCounter() << ", derived_data_set is " << derived_data_set << " source data is " << data_source->getData();
    // If source's data has changed (check by checking its counter), recalculate our derived data.
    if(!derived_data_set || (data_updated_counter != data_source->getUpdateCounter()))
    {
      qDebug() << "\t recalculating derived data...";
      derived_data = do_calculation(data_source->getData());
      qDebug() << "\t our derived data is now" << derived_data;
      data_updated_counter = data_source->getUpdateCounter();
      derived_data_set = true;
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
#ifdef LAZY
  // Signal received from DataSource.  Instead of recalculating our derived data here,
  // we relay the signal to any listeners, which can call getData() to get the calculated 
  // or cached derived data. (IF no listener calls getData() data will not be calculated.)
  void dataSourceUpdated([[maybe_unused]] int source_data) {
    emit notifyDataUpdated();
  }
#else

  // Signal received from DataSource, calculate our "derived" data.
  void dataSourceUpdated(int source_data) {
    //qDebug() << name.c_str() << ".dataSourceUpdated_NonLazy() signal received: source data is " << source_data << ", recalculating derived data...";
    derived_data = do_calculation(source_data);
    //qDebug() << "\tour derived data is now" << derived_data << ". Emitting update signal...";
    emit notifyDataUpdated();

  }
#endif
};


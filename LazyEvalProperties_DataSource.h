#pragma once

#include <QObject>
#include <QDebug>

class DataSource : public QObject {
  Q_OBJECT
  Q_PROPERTY(int data READ getData WRITE updateData NOTIFY notifyDataUpdated)
private:
  int counter {0};
  int data;

public:
  explicit DataSource(int init_data) : data(init_data) {}
  DataSource() = delete;
  int getData() { return data; }
  int getUpdateCounter() { return counter; }
  virtual ~DataSource() {}

  // Externally triggered by UI (see property above), timer, network, hardware device, etc.
  void updateData(int newdata) {
    data = newdata;
    ++counter;
    qDebug() << "DataSource: data update triggered. Data is now " << data << " , counter is now " << counter;
    emit notifyDataUpdated(data);
  }

signals:
  void notifyDataUpdated(int data);

};


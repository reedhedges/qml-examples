#pragma once

#include <QObject>
#include <QDebug>

class DataSource : public QObject {
  Q_OBJECT
private:
  int counter {0};
  int data {0};

public:
  int getData() { return data; }

  int getUpdateCounter() { return counter; }

  virtual ~DataSource() {}

  // Externally triggered by UI, timer, network, hardware device, etc.
  void updateData(int newdata) {
    data = newdata;
    ++counter;
    qDebug() << "DataSource: data update triggered. Data is now " << data << " , counter is now " << counter;
    emit notifyDataUpdated(data);
  }

signals:
  void notifyDataUpdated(int data);

};


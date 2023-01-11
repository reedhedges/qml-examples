
#pragma once

#include <QObject>
#include <QTimer>
#include <thread>
#include <chrono>
using namespace std::literals::chrono_literals;

class TimerExample : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool running MEMBER mrunning NOTIFY runningChanged)

private:
  bool mrunning = false;
  void setRunning(bool val)
  {
      mrunning = val;
      emit runningChanged(val);
  }

public:

  TimerExample() : QObject(nullptr) {}

  ~TimerExample() = default;

  Q_INVOKABLE void beginWork()
  {
    qDebug("beginWork() called...");
    setRunning(true);
    QTimer::singleShot(110, this, 
      [this] ()
      {
        qDebug("Doing work, after timer fired...");
        this->doWork();
        setRunning(false);
        qDebug("Done with work.");
      }
    );
  }

 private:
  void doWork()
  {
    // dummy workload, sleep for 5 seconds
    std::this_thread::sleep_for(5s);
  }



signals:
  void runningChanged(bool val);
};


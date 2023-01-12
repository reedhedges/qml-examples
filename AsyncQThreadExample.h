
#pragma once

#include "qdebug.h"
#include <QObject>
#include <QThread>
#include <iostream>
#include <QTimerEvent>

// For some reason a class derived from QObject can't be nested in another class, so we declare it out here in a private namespace:
class QThreadExample;
namespace qthread_example_private
{
    class Thread : public QThread
    {
      Q_OBJECT

    public:
      explicit Thread(QThreadExample *ex);
      ~Thread();

    public slots:
      void request_error();
      void request_stall();

    signals:
      void progress(int percent);
      void error(int errorcode);

    private:
      virtual void timerEvent(QTimerEvent *evt) override;
      int percentDone = 0;
      QThreadExample *example;
    };
}


class QThreadExample : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool running MEMBER mrunning NOTIFY runningChanged)
  Q_PROPERTY(float progress READ getProgress NOTIFY progressChanged)
  Q_PROPERTY(bool haveError READ haveError NOTIFY progressChanged)

  bool mrunning = false;
  std::optional<float> mprogress = std::nullopt;
  std::optional<int> merror = std::nullopt;

public:
  float getProgress()
  {
      if(mprogress) return mprogress.value();
      else return -1.0;
  }

  bool haveProgress()
  {
      return mprogress.has_value();
  }

  bool haveError()
  {
      return merror.has_value();
  }

private:
  std::unique_ptr<qthread_example_private::Thread> thread;

signals:
  void runningChanged(bool val);
  void progressChanged(float percent);

public:

  QThreadExample() : QObject(nullptr) {}

  ~QThreadExample() = default;

  Q_INVOKABLE void beginWork()
  {
    qDebug() << "beginWork() called. Main thread ID: " << QThread::currentThreadId();

    // Set or replace and start new thread:
    using namespace qthread_example_private;

    // We don't really need to use a unique_ptr to restart Thread, we could have a persistent Thread member that's reused
    // (or replace in-place to reset), but this is one way to do it:

    if(thread) {
        qDebug("Already have a thread, replacing it (Quit and wait...)");
        thread->quit();
        thread->wait();
    }
    thread = std::make_unique<Thread>(this);

    // Communicate with the new thread by sending and receiving signals:

    QObject::connect(thread.get(), &Thread::progress, this,
        [this](int percent)
        {
            mprogress = percent / 100.0;
            emit progressChanged(mprogress.value());
        },
        Qt::QueuedConnection
    );

    QObject::connect(thread.get(), &QThread::started, this,
        [this]()
        {
            qDebug() << "Thread started signal received by QThreadExample on thread " << QThread::currentThreadId();
            mrunning = true;
            emit runningChanged(true);
        },
        Qt::QueuedConnection
    );

    QObject::connect(thread.get(), &QThread::finished, this,
        [this]()
        {
            qDebug() << "Thread finished signal received by QThreadExample on thread " << QThread::currentThreadId();
            mprogress = std::nullopt;
            mrunning = false;
            emit runningChanged(false);
        },
        Qt::QueuedConnection
    );

    QObject::connect(thread.get(), &Thread::error, this,
        [this](int errorcode)
        {
            qDebug() << "Thread sent error signal with errorcode " << errorcode;
            this->merror = errorcode;
            emit progressChanged(mprogress.value());
        },
        Qt::QueuedConnection
    );

    // Emit signal from this object, receive in thread:
    QObject::connect(this, &QThreadExample::requestStallInThread, thread.get(), &Thread::request_stall);
    QObject::connect(this, &QThreadExample::requestErrorInThread, thread.get(), &Thread::request_error);

    thread->start();
  }

  // Called from UI: send signal to thread:

  Q_INVOKABLE void stallInThread()
  {
      emit requestStallInThread();
  }

  Q_INVOKABLE void errorInThread()
  {
      emit requestErrorInThread();
  }

signals: //(internal only)
  // These are only sent to thread:
  void requestStallInThread();
  void requestErrorInThread();


};


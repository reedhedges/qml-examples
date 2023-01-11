
#pragma once

#include "qdebug.h"
#include <QObject>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;
#include <memory>
#include <optional>

// One approach to managing the thread is to encapsulate everything related to the thread in a new object:
// the std::jthread object, and the doWork() function that runs in the thread.
// This object is a QObject and communicates with the ThreadExample object safely vi Qt signal.
// In this design, ThreadExample is an interface from the UI that creates/destroys the thread. This internal
// Thread objects manages the actual thread "contents".
// (Note: unfortunately we need to define functions separately in .cpp file (namely any that needs full
// ThreadExample declaration.)

class ThreadExample;
namespace thread_example_private
{
    class Thread : public QObject
    {
        Q_OBJECT
    public:
      Thread(ThreadExample *ex);
      ~Thread();
      void run(std::stop_token stop);
      void operator()(std::stop_token stop);
      void request_stop();

    signals:
      void isRunning(bool val);
      void progress(int percent);
      void threadEnding();

    private:
      ThreadExample *example;
      std::jthread jthread; // note C++20 required for jthread
    };
}


class ThreadExample : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool running MEMBER mrunning NOTIFY runningChanged)
  Q_PROPERTY(float progress READ getProgress NOTIFY progressChanged)

  // The new thread will access this via setRunning() signal. Alternatively, we could protect data like this with a mutex or atomic access.
  bool mrunning = false;

  std::optional<float> mprogress = std::nullopt;

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

public slots:
  void setRunning(bool val)
  {
      mrunning = val;
      emit runningChanged(val);
  }


private:

  friend thread_example_private::Thread;
  std::unique_ptr<thread_example_private::Thread> thread;

signals:
  void runningChanged(bool val);
  void progressChanged(float percent);

public:

  ThreadExample() : QObject(nullptr) {}

  ~ThreadExample() = default;

  Q_INVOKABLE void beginWork()
  {
    qDebug("beginWork() called...");

    // Set or replace `thread`. Note: the std::jthread object will block in its destructor waiting for any previous thread to end.

    // Approach one: Use jthread directly. In the function running in the thread, we must ensure thread safety in any calls or access to
    // other objects.
    /*
    if(thread) thread->request_stop();
    thread = std::make_unique<std::jthread>(
                [this] ()
                {
                    qDebug("Thread running...");
                    this->setRunning(true); // we might need to protect mrunning member with a mutex or atomic access
                    std::this_thread::sleep_for(5s); // TODO: we should check for requested cancellation while doing work.
                    this->setRunning(false);
                }
    );
    */

    using namespace thread_example_private;

    // Approach two: Use a QObject which can send Qt signals from the new thread:
    if(thread) thread->request_stop();
    thread = std::make_unique<Thread>(this);

    QObject::connect(thread.get(), &Thread::progress, this,
                        [this](int percent)
                        {
                            mprogress = percent / 100.0;
                            emit progressChanged(mprogress.value());
                        }
    );

    QObject::connect(thread.get(), &Thread::threadEnding, this,
                        [this]()
                        {
                            qDebug() << "In ThreadExample interface, got thread ending signal"  ;
                            mprogress = std::nullopt;
                        }
    );

  }

};


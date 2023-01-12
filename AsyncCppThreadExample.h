
#pragma once

#include "qdebug.h"
#include <QObject>
#include <thread>
#include <chrono>
using namespace std::chrono_literals;
#include <memory>
#include <optional>
#include <iostream>
#include <atomic>
#include <system_error>

// One approach to managing the thread is to encapsulate everything related to the thread in a new object:
// the std::jthread object, and the doWork() function that runs in the thread.
// This object is a QObject and communicates with the ThreadExample object safely vi Qt signal.
// In this design, ThreadExample is an interface from the UI that creates/destroys the thread. This internal
// Thread objects manages the actual thread "contents".
// (Note: unfortunately we need to define functions separately in .cpp file (namely any that needs full
// ThreadExample declaration.)
// TODO modify to make sure signals are really being received in the thread


// Object created for the thread:
// For some reason a class derived from QObject can't be nested in another class, so we declare it out here in a private namespace:
class CppThreadExample;
namespace thread_example_private
{
    class Thread : public QObject
    {
      Q_OBJECT

    public:
      Thread(CppThreadExample *ex);
      ~Thread();

      void request_stop();

    private:
      // To send requests into the running thread, we use an inverted atomic_flag that provides set() and test_and_clear() instead of clear() and test_and_set().
      // (If the thread was a QThread, we could send a Qt signal to a QObject owned by that QThread; see AsyncQThreadExample.h)
      // (Could this be generalized with a set of atomic flags, associated mutex-protected argument structs/tuples/argument list objects?, and API to set/get these?)
      class atomic_signal_flag {
      private:
          std::atomic_flag flag;
      public:
          atomic_signal_flag() noexcept {
              test_and_clear();
          }
          inline bool test_and_clear() noexcept {
              return ! flag.test_and_set();
          }
          inline void set() noexcept {
              flag.clear();
          }
      };
      atomic_signal_flag request_error_flag;
      atomic_signal_flag request_stall_flag;
    public:
      void request_error() {
          request_error_flag.set();
      }
      void request_stall() {
          request_stall_flag.set();
      }

    signals:
      void isRunning(bool val);
      void progress(int percent);
      void threadEnding();
      void error(std::error_code err); ///< @sa thread_example_errc

    private:
      void run(std::stop_token stop);
      CppThreadExample *example;
      std::jthread jthread; // note C++20 required for jthread
    };

    // Error codes. @sa Thread:error() signal.
    enum class thread_example_errc {
        ExampleError = 1,
        AnotherExampleError = 2
    };
}

// Let our error code type work with std::system_error:
// (You don't need to use std::system_error to return errors, you could return some other value with the error signal if desired)
namespace std
{
    template <> struct is_error_code_enum<thread_example_private::thread_example_errc> : true_type {};
}

class CppThreadExample : public QObject
{
  Q_OBJECT

  Q_PROPERTY(bool running MEMBER mrunning NOTIFY runningChanged)
  Q_PROPERTY(float progress READ getProgress NOTIFY progressChanged)
  Q_PROPERTY(bool haveError READ haveError NOTIFY progressChanged)

  // The new thread will access this via setRunning() signal. Alternatively, we could protect data like this with a mutex or atomic access.
  bool mrunning = false;
  std::optional<float> mprogress = std::nullopt;
  std::optional<std::error_code> merror = std::nullopt;

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

private slots:
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

  CppThreadExample() : QObject(nullptr) {}

  ~CppThreadExample() = default;

  Q_INVOKABLE void beginWork()
  {
    qDebug("beginWork() called...");
    std::cerr << "Main thread ID: " << std::hex << std::this_thread::get_id() << '\n';

    // Set or replace `thread` (new thread is started).
    // Note: the std::jthread object will block in its destructor waiting for any previous thread to end.


    using namespace thread_example_private;

    if(thread) thread->request_stop();
    thread = std::make_unique<Thread>(this);

    QObject::connect(thread.get(), &Thread::progress, this,
        [this](int percent)
        {
            if(percent == -1)
            {
                mprogress = std::nullopt;
                emit progressChanged(-1);
            }
            else
            {
                mprogress = percent / 100.0;
                emit progressChanged(mprogress.value());
            }
        },
        Qt::QueuedConnection
    );

    QObject::connect(thread.get(), &Thread::error, this,
        [this](std::error_code err)
        {
            merror = err;
            emit progressChanged(-1);
        },
    Qt::QueuedConnection);

    QObject::connect(thread.get(), &Thread::threadEnding, this,
        [this]()
        {
            qDebug() << "In ThreadExample interface, got thread ending signal"  ;
            std::cerr << "Thread ending signal received on thread " << std::this_thread::get_id() << '\n';
            mprogress = std::nullopt;
            merror = std::nullopt;
        },
        Qt::QueuedConnection
    );

    // You can use a setter method instead of lambda if you really want to (but don't confuse internal setter with external Q_PROPERTY setter; recommend a naming convention and use of public/private to indicate the difference)
    connect(thread.get(), &Thread::isRunning, this, &CppThreadExample::setRunning, Qt::QueuedConnection);



  }

  Q_INVOKABLE void requestStallInThread()
  {
      thread->request_stall();
  }

  Q_INVOKABLE void requestErrorInThread()
  {
      thread->request_error();
  }



};


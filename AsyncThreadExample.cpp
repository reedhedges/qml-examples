

#include "AsyncThreadExample.h"
#include "qdebug.h"

#include <functional> // for bind_front

thread_example_private::Thread::Thread(ThreadExample *ex) :
  example(ex),
  //jthread(std::bind_front(&Thread::run, this))
  // or if you prefer:  jthread([this](){this->run();})
  jthread(*this)
{
  connect(this, &Thread::isRunning, example, &ThreadExample::setRunning);
}

thread_example_private::Thread::~Thread()
{
  // disconnect signal?
}

void thread_example_private::Thread::operator()(std::stop_token stop)
{
  run(stop);
}

void thread_example_private::Thread::run(std::stop_token stop)
{
  qDebug("Thread running...");
  emit isRunning(true);
  // dummy workload, sleep for 5 seconds
  for(auto i = 1; i <= 100; ++i)
  {
      if(stop.stop_requested())
      {
          qDebug("...thread stop requested!");
          break;
      }
      std::this_thread::sleep_for(50ms);
      emit progress(i);
  }
  emit threadEnding();
  emit isRunning(false);
}

void thread_example_private::Thread::request_stop()
{
    // just a wrapper to jthread::request_stop().
  jthread.request_stop();
}



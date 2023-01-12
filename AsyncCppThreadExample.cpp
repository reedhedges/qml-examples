

#include "AsyncCppThreadExample.h"
#include <functional> // for bind_front
#include <iostream>
#include <cassert>


namespace thread_example_private {

struct thread_example_error_category_t : std::error_category
{
    const char* name() const noexcept override
    {
        return "thread_example";
    }

    std::string message(int errnum) const override
    {
        switch(static_cast<thread_example_errc>(errnum))
        {
            case thread_example_errc::ExampleError:
                return "example error";
            case thread_example_errc::AnotherExampleError:
                return "another example error";
        }
        return "unknown error!";
    }

};

// global singleton instance
const thread_example_error_category_t thread_example_error_category_inst {};

} // namespace

std::error_code make_error_code(thread_example_private::thread_example_errc e)
{
    return {static_cast<int>(e), thread_example_private::thread_example_error_category_inst};
}


thread_example_private::Thread::Thread(CppThreadExample *ex) :
  example(ex),
  jthread(std::bind_front(&Thread::run, this))
  // or if you prefer:  jthread([this](){this->run();})
{
}

thread_example_private::Thread::~Thread()
{
  // disconnect signal? other cleanup?
}


void thread_example_private::Thread::run(std::stop_token stop)
{

  qDebug("Thread running...");
  std::cerr << "New thread ID=" << std::hex << jthread.get_id() << " this_thread ID=" << std::this_thread::get_id() << '\n';
  assert(jthread.get_id() == std::this_thread::get_id());

  emit isRunning(true);


  // dummy workload, sleep for 5 seconds:
  for(auto i = 1; i <= 100; ++i)
  {
      if(stop.stop_requested())
      {
          qDebug("...thread stop requested!");
          break;
      }
      if(request_error_flag.test_and_clear())
      {
          qDebug("...thread error requested");
          std::cerr << "Thread " << std::this_thread::get_id() << " got error request\n";
          // could instead create a try/catch around the whole method thath emits the error from the exception (could just pass the exception as the signal argument), and throw an example exception here.
          emit error(make_error_code(thread_example_errc::ExampleError));
          break;
      }
      if(request_stall_flag.test_and_clear())
      {
          qDebug("...thread stall requested");
          std::cerr << "Thread " << std::this_thread::get_id() << " got stall request\n";
          emit progress(-1);
          std::this_thread::sleep_for(3s); // todo use better state machine management and don't hang here.
      }
      std::this_thread::sleep_for(50ms);
      emit progress(i);
  }

  std::cerr << "Sending threadEnding signal from thread " << std::this_thread::get_id() << '\n';
  emit threadEnding();
  emit isRunning(false);
}

void thread_example_private::Thread::request_stop()
{
    // just a wrapper to jthread::request_stop().
  jthread.request_stop();
}




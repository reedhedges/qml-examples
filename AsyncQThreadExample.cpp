

#include "AsyncQThreadExample.h"
#include "qdebug.h"

#include <functional> // for bind_front

#include <iostream>
#include <cassert>

qthread_example_private::Thread::Thread(QThreadExample *ex) :
  QThread(),
  example(ex)
{
    startTimer(50);
}

qthread_example_private::Thread::~Thread()
{
  // disconnect signal? other cleanup?
}


void qthread_example_private::Thread::timerEvent([[maybe_unused]] QTimerEvent *evt)
{
    percentDone += 1;
    if(percentDone >= 100)
        this->quit(); // end thread
    emit progress(percentDone);
}

//void qthread_example_private::Thread::request_stop()
//{
//    qDebug() << "received request_stop signal on thread id " << QThread::currentThreadId();
//    QThread::quit();
//}

void qthread_example_private::Thread::request_error() {
    qDebug() << "received request_error signal on thread id " << QThread::currentThreadId();
    emit error(23);
}

void qthread_example_private::Thread::request_stall() {
    qDebug() << "received request_stall signal on thread id " << QThread::currentThreadId();
    // TODO
}




/* This example shows ways of handling a signal asynchronously, and how it affects the UI thread:
    1. Timer Hack: Use QTimer to queue an event for at least 100ms in the future. The UI thread is still blocked when
        the timer expires and the timer event is handled, but it gives the UI time to do work between the user action
        that initiated it, and handling the event. This can give time for dialog boxes to close, etc.
    2. Start short task thread using C++ standard library thread API.
    3. (not implemented yet) Start short task thread using Qt thread API.
    4. (not implemented yet) At program initialization, start a long running background thread that is normally idle, but which can
        handle Qt signals sent from the UI.

    The interface between the QML UI and the C++ code is a small controller class with Q_INVOKABLE methods. This is
    my preferred design when the general design and user's mental model of the application is to explicitly initiate
    some action; but we could respond to property changes as well if that makes more sense.

    In all cases, we start and stop a "busy" indicator via a boolean value; maybe other progress info is also provided.

    In the thread cases Qt signals are used to safely communicate between the new thread and the main (UI) thread.

    You could adapt these examples into re-usable utility classes that can be reused mulitple times in an application,
    especially an easy-to use and easy to extend version of the ThreadExample, that reduces the boiler plate needed
    for each property/signal, thread management, etc.

    TODO try other asynchronous patterns/apis; promise based, etc

    TODO show example with some kind of thread container or manager object that can do 
    things like automatically join (wait for) threads on destruction, or signal threads to 
    abort, on destruction, etc. (when these aren't normally done by whatever thread API we are using)

*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>

#include "AsyncTimerExample.h"
#include "AsyncThreadExample.h"

int main(int argc, char **argv)
{
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;



  // Catch objectCreationFailed signal emitted by engine:
  // Note, objectCreationFailed() signal added in Qt 6.4. For previous versions, must connect to objectCreated() signal but check for 
  // NULL object pointer.
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        [] (const QUrl &objUrl) 
        {
            qDebug() << "Error loading QML object: " << qPrintable(objUrl.errorString());
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

  // Pre Qt 6.4: Catch objectCreated signal emitted by engine for each QML object creation. Check for NULL object pointer indicating error, and exit:
//  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
//        [] (const QObject *const obj, const QUrl &objUrl) 
//         {
//          if(!obj) {
//            qDebug() << "Error loading QML object: " << qPrintable(objUrl.errorString());
//            QCoreApplication::exit(-1);
//          }
//        },
//        Qt::QueuedConnection);

  // Signal emitted after loading QML with list of warnings:
  QObject::connect(&engine, &QQmlApplicationEngine::warnings, &app,
        [] (const QList<QQmlError>& warnings) 
        {
            qDebug("Warnings generated while loading QML:");
            for(auto& e : warnings)
                qDebug("\tQML Warning: %s:%d: %s", qPrintable(e.url().toString()), e.line(), qPrintable(e.toString()));
            // QCoreApplication::exit(-2);
        },
        Qt::QueuedConnection);

  TimerExample timer_example;
  qmlRegisterSingletonInstance("dev.dogsbody.AsyncExamples", 1, 0, "TimerExample", &timer_example);

  ThreadExample thread_example;
  qmlRegisterSingletonInstance("dev.dogsbody.AsyncExamples", 1, 0, "ThreadExample", &thread_example);


  // Load QML:
  engine.load(u"qrc:/dev/dogsbody/AsyncExamples/Async.qml"_qs);


  // Run:
  return app.exec();
}



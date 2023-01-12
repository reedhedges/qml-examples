

/* This example shows ways of handling a signal asynchronously, and how it affects the UI thread:
    1. Timer Hack: Use QTimer to queue an event for at least 100ms in the future. The UI thread is still blocked when
        the timer expires and the timer event is handled, but it gives the UI time to do work between the user action
        that initiated it, and handling the event. This can give time for dialog boxes to close, etc.
    2. Start short task thread using C++ standard library thread API.
    3. (not implemented yet) Start short task thread using Qt thread API.
    4. (not implemented yet) At program initialization, start a long running background thread that is normally idle, but which can
        handle Qt signals sent from the UI.

    For each case, the interface between the QML UI and the C++ code is a controller class with Q_INVOKABLE methods and properties.

    In all cases, we start and stop a "busy" indicator via a boolean value; we receive data updates and errors from the thread via Qt signals
    (which are posted to the controller/interface QObject's signal queue, so are received in the main UI thread.)  To send
    signals to the thread, we either use Qt signals (when the thread is QThread and a QObject with its own event loop and signal
    queue, or atomic signal flags for C++ standard threads -- a more sophisticated message passing system could be used for
    that if desired.)

    You could adapt these examples into re-usable utility classes that can be reused mulitple times in an application,
    especially an easy-to use and easy to extend version of the ThreadExample, that reduces the boiler plate needed
    for each property/signal, thread management, etc. Maybe there's a way to use QObject/QMetaObject, Q_PROPERTY,
    etc. to generate/provide mechanism for easy updating of properties from threads via signals, and sending signals to thread (setting properties or sending signals)? (If we use
    Q_PROPERTY in the thread's QObject, will much of this be there for us? But we also need to declare the signals
    and connections, which is also boilerplate used for everything that could be automated). Also consider what data is sent back while the
    thread is running (updates, errors, status/log messages), and what could be returned or made available to the initiator after the thread is completed.

    TODO try other asynchronous patterns/apis; promise based, etc

*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>

#include "AsyncTimerExample.h"
#include "AsyncCppThreadExample.h"
#include "AsyncQThreadExample.h"

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

  CppThreadExample cppthread_example;
  qmlRegisterSingletonInstance("dev.dogsbody.AsyncExamples", 1, 0, "CppThreadExample", &cppthread_example);

  QThreadExample qthread_example;
  qmlRegisterSingletonInstance("dev.dogsbody.AsyncExamples", 1, 0, "QThreadExample", &qthread_example);


  // Load QML:
  engine.load(u"qrc:/dev/dogsbody/AsyncExamples/Async.qml"_qs);


  // Run:
  return app.exec();
}



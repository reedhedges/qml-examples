#include <string>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QList>
#include <QElapsedTimer>

#include "LazyEvalProperties_DataSource.h"
#include "LazyEvalProperties_DataItem.h"

static const size_t num_data_items = 25; // how many total data items to create
static const size_t num_data_items_enabled = 10; // how many data items that do reads (e.g. are visible)


int test_main()
{
  DataSource data_source;

  QList<DataItem> data_items;
  for(int i = 0; i < num_data_items; ++i)
  {
    std::string name("DataItem");
    name += std::to_string(i);
    data_items.emplace_back(name, &data_source);
  }

  QElapsedTimer timer;
  QElapsedTimer totaltimer;
  timer.start();
  totaltimer.start();

  qDebug() << "Setting source data to 1...";
  data_source.updateData(1);

  qDebug() << "Update of source data took " << timer.elapsed() << "ms";

  timer.restart();

  for(int i = 0; i < num_data_items_enabled; ++i) {
        qDebug() << "read #" << i << ":" << data_items[i].getData();
  }
  qDebug() << "First reads took " << timer.elapsed() << "ms";

  timer.restart();

  for(int i = 0; i < num_data_items_enabled; ++i) {
        qDebug() << "read #" << i << ":" << data_items[i].getData();
  }

  qDebug() << "Second reads took " << timer.elapsed() << "ms";
  qDebug() << "Total time 1 = " << totaltimer.elapsed() << "ms";

  timer.restart();
  totaltimer.restart();

  qDebug() << "Setting source data to 2...";
  data_source.updateData(2);

  qDebug() << "Source update took " << timer.elapsed() << "ms";

  timer.restart();

  for(int i = 0; i < num_data_items_enabled; ++i) {
        qDebug() << "read #" << i << ":" << data_items[i].getData();
  }

  qDebug() << "First reads took " << timer.elapsed() << "ms";

  timer.restart();
  
  for(int i = 0; i < num_data_items_enabled; ++i) {
        qDebug() << "read #" << i << ":" << data_items[i].getData();
  }
  
  qDebug() << "Second reads took " << timer.elapsed() << "ms";
  qDebug() << "Total time 2 = " << totaltimer.elapsed() << "ms";
  return 0;
}

int gui_main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/LazyEvalProperties/LazyEvalProperties.qml"_qs);

    // Abort on error loading main QML component:
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    // QML Warnings:
    QObject::connect(&engine, &QQmlApplicationEngine::warnings,
                     &app, [](const QList<QQmlError>& warnings) {
        qDebug("Warnings encountered loading QML document (see above)!");
        QCoreApplication::exit(-2);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}

int main(int argc, char *argv[])
{
  return test_main();
  // return gui_main(argc, argv);
}

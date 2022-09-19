#include <string>
#include <string_view>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QList>
#include <QElapsedTimer>

#include "LazyEvalProperties_DataSource.h"
#include "LazyEvalProperties_DataItem.h"



// Testing basic method of "lazy evaluation" or "evaluation on read" of properties.  This moves calculations to the GUI thread however, potentially slowing it down.

// See other examples to only calculate data if included in a certain data model or if a certain QML element is enabled (has true visible property).


static const size_t num_data_items = 50; // how many total data items to create


int test_main(DataSource& data_source, QList<DataItem>& data_items)
{
  static const size_t num_data_items_enabled = 10; // how many data items to read in test_main()

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

int gui_main(int argc, char *argv[], DataSource& data_source, QList<DataItem>& data_items)
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

    // Properties
    // Need a list of QObject pointers for QML data model
    QList<QObject*> data_items_list_model;
    data_items_list_model.reserve(data_items.size());
    for(auto& d : data_items)
    {
        data_items_list_model.append(&d);
    }
    engine.rootContext()->setContextProperty("dataSource", &data_source);
    engine.rootContext()->setContextProperty("dataItemsListModel", QVariant::fromValue(data_items_list_model));

    engine.load(url);

    return app.exec();
}

int main(int argc, char *argv[])
{

    DataSource data_source(0);

    QList<DataItem> data_items;
    data_items.reserve(num_data_items);
    for(int i = 0; i < num_data_items; ++i)
    {
      std::string name("DataItem");
      name += std::to_string(i);
      data_items.emplace_back(name, &data_source);
    }


    if(argc > 1 && std::string_view(argv[1]) == "--test")
        return test_main(data_source, data_items);
    else
        return gui_main(argc, argv, data_source, data_items);
}

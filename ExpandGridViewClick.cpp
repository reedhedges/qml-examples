#include <QGuiApplication>
#include <QQmlApplicationEngine>


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/ExpandGridViewClick/ExpandGridViewClick.qml"_qs);

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

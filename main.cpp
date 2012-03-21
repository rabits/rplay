#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative>
#include <QSettings>
#include "qmlapplicationviewer.h"

#include "src/ctree.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Rabits");
    QCoreApplication::setOrganizationDomain("rabits.ru");
    QCoreApplication::setApplicationName("rplay");

    QSettings settings;

    QScopedPointer<QApplication> app(createApplication(argc, argv));

    qmlRegisterType<ListModel>("ListModels", 1, 0, "ListModel");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    CTree ctree(&settings);

    QDeclarativeContext* context = viewer.rootContext();
    context->setContextProperty("ctree", &ctree);


    viewer.setMainQmlFile(QLatin1String("qml/rplay/main.qml"));
    viewer.showExpanded();
    return app->exec();
}

#include <QtGui/QApplication>
#include <QtDeclarative>
#include "qmlapplicationviewer.h"

#include "src/cplayer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    qDebug("[rPlay] Init rPlay v"PROJECT_VERSION);

    QCoreApplication::setOrganizationName("Rabits");
    QCoreApplication::setOrganizationDomain("rabits.ru");
    QCoreApplication::setApplicationName("rplay");
    QCoreApplication::setApplicationVersion(PROJECT_VERSION);

    QScopedPointer<QApplication> app(createApplication(argc, argv));

    qmlRegisterType<ListModel>("ListModels", 1, 0, "ListModel");

    QmlApplicationViewer viewer;

    qDebug("[rPlay] Creating context");
    CPlayer::getInstance()->initContext(viewer, &app);

    qDebug("[rPlay] Loading qml");
    viewer.setMainQmlFile(QLatin1String("qml/rplay/main.qml"));

    CPlayer::getInstance()->initRoot(viewer);

    qDebug("[rPlay] Init done, starting");

    viewer.showExpanded();
    return app->exec();
}

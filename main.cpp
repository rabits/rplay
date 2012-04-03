#include <QtGui/QApplication>
#include <QtDeclarative>
#include "qmlapplicationviewer.h"

#include "src/cplayer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Rabits");
    QCoreApplication::setOrganizationDomain("rabits.ru");
    QCoreApplication::setApplicationName("rplay");
    QCoreApplication::setApplicationVersion("0.1.0");

    QScopedPointer<QApplication> app(createApplication(argc, argv));

    qmlRegisterType<ListModel>("ListModels", 1, 0, "ListModel");

    QmlApplicationViewer viewer;

    CPlayer::getInstance()->initContext(viewer);

    viewer.setMainQmlFile(QLatin1String("qml/rplay/main.qml"));
    viewer.showExpanded();
    return app->exec();
}

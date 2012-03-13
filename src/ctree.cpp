#include "ctree.h"

CTree::CTree(QSettings* settings, QObject *parent)
    : QObject(parent)
    , m_settings(settings)
{
}

QObject* CTree::treeContent(QString path)
{
    ListModel* out = new ListModel(new CTreeItem(), parent());
    QDir dir = QDir(m_settings->value("ctree/root_music", QString(QDir::homePath())).toString() + path);
    QStringList list = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    for( QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it )
    {
        out->appendRow(new CTreeItem(*it, path + "/" + *it, QString("folder"), it->length(), this));
    }

    list = dir.entryList(QDir::Files);
    for( QStringList::const_iterator it = list.constBegin(); it != list.constEnd(); ++it )
    {
        out->appendRow(new CTreeItem(*it, path + "/" + *it, QString("file"), it->length(), this));
    }

    return out;
}

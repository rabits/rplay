#ifndef CTREE_H
#define CTREE_H

#include <QObject>
#include <QListData>
#include <QDir>

#include "model/ctreeitem.h"

class CTree : public QObject
{
    Q_OBJECT

public:
    explicit CTree(QObject *parent = 0);
    Q_INVOKABLE ListModel* treeContent(QString path = QString("/"));
    Q_INVOKABLE QString parentDir(QString path);
    Q_INVOKABLE inline QString getName(QString path) { return QDir(libraryRoot(path)).dirName(); }

    QString libraryRoot(QString path);
    QString libraryRootRemove(QString path);

    Q_INVOKABLE void        shuffleGenerateList();
    Q_INVOKABLE inline void shuffleClearList() { m_shuffle_notplayed_list.clear(); }
    QStringList findFiles(QStringList paths);

    QString findNextFile(QString path);
    QString findFirstFile(QDir dir, QString prev_dir = QString());

private:
    QStringList m_music_filters;
    QStringList m_shuffle_notplayed_list;
};

#endif // CTREE_H

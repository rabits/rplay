#ifndef CTREE_H
#define CTREE_H

#include <QObject>
#include <QListData>
#include <QDir>

#include "model/ctreeitem.h"

class CTree : public QObject
{
    Q_OBJECT
private:

public:
    explicit CTree(QObject *parent = 0);
    Q_INVOKABLE ListModel* treeContent(QString path = QString("/"));
    Q_INVOKABLE QString parentDir(QString path);
    Q_INVOKABLE QString getName(QString path);
    Q_INVOKABLE QString findCover(QString path);
    QString findNextFile(QString path);
    QString findFirstFile(QDir dir, QString prev_dir = QString());
    
signals:
    
public slots:
    
};

#endif // CTREE_H

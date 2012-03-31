#ifndef CTREE_H
#define CTREE_H

#include <QObject>
#include <QListData>
#include <QDir>

#include "ctreeitem.h"

class CTree : public QObject
{
    Q_OBJECT
private:

public:
    explicit CTree(QObject *parent = 0);
    Q_INVOKABLE ListModel* treeContent(QString path = QString("/"));
    QString findNextFile(QString path);
    QString findFirstFile(QDir dir, QString prev_dir = QString());
    
signals:
    
public slots:
    
};

#endif // CTREE_H

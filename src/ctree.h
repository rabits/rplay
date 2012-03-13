#ifndef CTREE_H
#define CTREE_H

#include <QObject>
#include <QListData>
#include <QDir>
#include <QSettings>

#include "ctreeitem.h"

class CTree : public QObject
{
    Q_OBJECT
private:
    QSettings* m_settings;

public:
    explicit CTree(QSettings*, QObject *parent = 0);
    Q_INVOKABLE QObject* treeContent(QString path = QString("/"));
    
signals:
    
public slots:
    
};

#endif // CTREE_H

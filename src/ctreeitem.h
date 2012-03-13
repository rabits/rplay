#ifndef CTREEITEM_H
#define CTREEITEM_H

#include <QObject>

#include "listmodel.h"

class CTreeItem : public ListItem
{
    Q_OBJECT
public:
    enum Roles {
        TitleRole = Qt::UserRole+1,
        PathRole,
        TypeRole,
        InsideRole
    };

    CTreeItem(QObject *parent = 0);
    explicit CTreeItem(const QString&, QString, QString, int, QObject *parent = 0);

    inline QString title() const { return m_title; }
    inline QString path() const { return m_path; }
    inline QString type() const { return m_type; }
    inline qint8 inside() const { return m_inside; }

    inline QString id() const { return m_path; }
    QVariant data(int role) const;
    QHash<int, QByteArray> roleNames() const;

private:
    QString m_title;
    QString m_path;
    QString m_type;
    qint8   m_inside;
    
signals:
    
public slots:
    
};

#endif // CTREEITEM_H

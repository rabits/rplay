#ifndef CTREEITEM_H
#define CTREEITEM_H

#include <QObject>

#include "../listmodel.h"

class CTreeItem : public ListItem
{
    Q_OBJECT
public:
    enum Roles {
        TitleRole = Qt::UserRole+1,
        PathRole,
        TypeRole,
        PictureRole,
        LevelRole,
        InsideRole
    };

    CTreeItem(QObject *parent = 0);
    explicit CTreeItem(const QString&, const QString, QString, QString, int, int, QObject *parent = 0);

    inline QString title() const { return m_title; }
    inline void title(const QVariant &value) { m_title = value.toString(); emit dataChanged(); }
    inline QString path() const { return m_path; }
    inline void path(const QVariant &value) { m_path = value.toString(); emit dataChanged(); }
    inline QString type() const { return m_type; }
    inline void type(const QVariant &value) { m_type = value.toString(); emit dataChanged(); }
    inline QString picture() const { return m_picture; }
    inline void picture(const QVariant &value) { m_picture = value.toString(); emit dataChanged(); }
    inline qint8 level() const { return m_level; }
    inline void level(const QVariant &value) { m_level = value.toInt(); emit dataChanged(); }
    inline qint8 inside() const { return m_inside; }
    inline void inside(const QVariant &value) { m_inside = value.toInt(); emit dataChanged(); }

    inline QString id() const { return m_path; }
    QVariant data(int role) const;
    bool data(int role, const QVariant &new_value);
    QHash<int, QByteArray> roleNames() const;

private:
    QString m_title;
    QString m_path;
    QString m_type;
    QString m_picture;
    qint8   m_level;
    qint8   m_inside;
    
signals:
    
public slots:
    
};

#endif // CTREEITEM_H

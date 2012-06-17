#ifndef CKEYVALUEITEM_H
#define CKEYVALUEITEM_H

#include <QObject>

#include "../listmodel.h"

class CKeyValueItem : public ListItem
{
    Q_OBJECT
public:
    enum Roles {
        KeyRole = Qt::UserRole+1,
        TitleRole,
        ValueRole,
        TypeRole
    };

    CKeyValueItem(QObject *parent = 0);
    explicit CKeyValueItem(const QString, const QString, const QString, const QString, QObject *parent = 0);

    inline QString key() const { return m_key; }
    inline void key(const QVariant &value) { m_key = value.toString(); emit dataChanged(); }
    inline QString title() const { return m_title; }
    inline void title(const QVariant &value) { m_title = value.toString(); emit dataChanged(); }
    inline QString value() const { return m_value; }
    inline void value(const QVariant &value) { m_value = value.toString(); emit dataChanged(); }
    inline QString type() const { return m_type; }
    inline void type(const QVariant &value) { m_type = value.toString(); emit dataChanged(); }

    inline QString id() const { return m_key; }
    QVariant data(int role) const;
    bool data(int role, const QVariant &new_value);
    QHash<int, QByteArray> roleNames() const;

private:
    QString m_key;
    QString m_title;
    QString m_value;
    QString m_type;
    
signals:
    
public slots:
    
};

#endif // CKEYVALUEITEM_H

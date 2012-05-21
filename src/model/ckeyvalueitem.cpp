#include "ckeyvalueitem.h"

CKeyValueItem::CKeyValueItem(QObject *parent) :
    ListItem(parent)
{
}

CKeyValueItem::CKeyValueItem(const QString key
                    , const QString title
                    , const QString value
                    , const QString type
                    , QObject *parent)
    : ListItem(parent)
    , m_key(key)
    , m_title(title)
    , m_value(value)
    , m_type(type)
{
}

QVariant CKeyValueItem::data(int role) const
{
    switch(role) {
    case KeyRole:
        return key();
    case TitleRole:
        return title();
    case ValueRole:
        return value();
    case TypeRole:
        return type();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> CKeyValueItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[KeyRole] = "key";
    names[TitleRole] = "title";
    names[ValueRole] = "value";
    names[TypeRole] = "type";
    return names;
}

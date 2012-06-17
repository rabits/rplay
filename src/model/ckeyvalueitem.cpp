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

bool CKeyValueItem::data(int role, const QVariant &new_value)
{
    switch(role) {
    case KeyRole:
        key(new_value);
        break;
    case TitleRole:
        title(new_value);
        break;
    case ValueRole:
        value(new_value);
        break;
    case TypeRole:
        type(new_value);
        break;
    default:
        return false;
    }
    return true;
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

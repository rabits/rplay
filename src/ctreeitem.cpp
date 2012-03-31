#include "ctreeitem.h"

CTreeItem::CTreeItem(QObject *parent) :
    ListItem(parent)
{
}

CTreeItem::CTreeItem(const QString &title, const QString path, QString type, QString picture, int inside, QObject *parent)
    : ListItem(parent)
    , m_title(title)
    , m_path(path)
    , m_type(type)
    , m_picture(picture)
    , m_inside(inside)
{
}

QVariant CTreeItem::data(int role) const
{
    switch(role) {
    case TitleRole:
        return title();
    case PathRole:
        return path();
    case TypeRole:
        return type();
    case PictureRole:
        return picture();
    case InsideRole:
        return inside();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> CTreeItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[TitleRole] = "title";
    names[PathRole] = "path";
    names[TypeRole] = "type";
    names[PictureRole] = "picture";
    names[InsideRole] = "inside";
    return names;
}

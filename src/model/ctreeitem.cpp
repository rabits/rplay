#include "ctreeitem.h"

CTreeItem::CTreeItem(QObject *parent) :
    ListItem(parent)
{
}

CTreeItem::CTreeItem(const QString &title
                    , const QString path
                    , QString type
                    , QString picture
                    , int level
                    , int inside, QObject *parent)
    : ListItem(parent)
    , m_title(title)
    , m_path(path)
    , m_type(type)
    , m_picture(picture)
    , m_level(level)
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
    case LevelRole:
        return level();
    case InsideRole:
        return inside();
    default:
        return QVariant();
    }
}

bool CTreeItem::data(int role, const QVariant &new_value)
{
    switch(role) {
    case TitleRole:
        title(new_value);
    case PathRole:
        path(new_value);
    case TypeRole:
        type(new_value);
    case PictureRole:
        picture(new_value);
    case LevelRole:
        level(new_value);
    case InsideRole:
        inside(new_value);
    default:
        return false;
    }
}

QHash<int, QByteArray> CTreeItem::roleNames() const
{
    QHash<int, QByteArray> names;
    names[TitleRole] = "title";
    names[PathRole] = "path";
    names[TypeRole] = "type";
    names[PictureRole] = "picture";
    names[LevelRole] = "level";
    names[InsideRole] = "inside";
    return names;
}

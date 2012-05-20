#include "ctree.h"
#include "cplayer.h"

CTree::CTree(QObject *parent)
    : QObject(parent)
{
}

QString CTree::findNextFile(QString path)
{
    path = CPlayer::getInstance()->setting("ctree/root_music").toString() + path;
    QDir dir = QFileInfo(path).absoluteDir();
    // Find in current directory
    QStringList list_files = dir.entryList(*(CPlayer::getInstance()->musicFilters()), QDir::Files);
    for( QStringList::const_iterator it = list_files.constBegin(); it != list_files.constEnd(); ++it )
    {
        QString fullpath = dir.absolutePath() + "/" + *it;
        if( fullpath == path )
        {
            if( (it+1) != list_files.constEnd() )
                return dir.absolutePath().replace(CPlayer::getInstance()->setting("ctree/root_music").toString(), QString())
                        + "/" + *(it+1);
        }
    }

    while( CPlayer::getInstance()->setting("ctree/root_music").toString() != dir.absolutePath() )
    {
        // Goto up
        QString previous_dir = dir.absolutePath();
        dir.cdUp();

        // Find next file in dirs upper then current
        QString output = findFirstFile(dir.absolutePath(), previous_dir);
        if( ! output.isEmpty() )
            return output.replace(CPlayer::getInstance()->setting("ctree/root_music").toString(), QString());
    }

    // Find first file in root - looping
    return findFirstFile(dir).replace(CPlayer::getInstance()->setting("ctree/root_music").toString(), QString());
}

QString CTree::findFirstFile(QDir dir, QString prev_dir)
{
    // Going to directories
    QStringList list_dirs = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);

    if( ! list_dirs.isEmpty() )
    {
        if( prev_dir.isEmpty() )
            return findFirstFile(dir.absolutePath() + "/" + list_dirs.first());

        for( QStringList::const_iterator it = list_dirs.constBegin(); it != list_dirs.constEnd(); ++it )
        {
            // Find current directory and use next dir, if it possible
            QString fullpath = dir.absolutePath() + "/" + *it;
            if( fullpath == prev_dir )
            {
                // We found currentdir - try to use next dirs
                if( (it+1) != list_dirs.constEnd() )
                {
                    for( ++it ; it != list_dirs.constEnd(); ++it )
                    {
                        QString next_in_folder = findFirstFile(dir.absolutePath() + "/" + *it);
                        if( ! next_in_folder.isEmpty() )
                            return next_in_folder;
                    }
                }
            }
        }
    }

    // If no dirs - use first file
    QStringList list_files = dir.entryList(*(CPlayer::getInstance()->musicFilters()), QDir::Files);
    if( list_files.isEmpty() )
        return QString();
    else
        return QString(dir.absolutePath() + "/" + list_files.first());
}

ListModel* CTree::treeContent(QString path)
{
    if( path == "/" )
        path = "";
    ListModel* out = new ListModel(new CTreeItem(), parent());
    QDir dir = QDir(CPlayer::getInstance()->setting("ctree/root_music").toString() + path);
    int level = path.split('/').count();

    QStringList list_dirs = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    QStringList list_files = dir.entryList(*(CPlayer::getInstance()->musicFilters()), QDir::Files);

    for( QStringList::const_iterator it = list_dirs.constBegin(); it != list_dirs.constEnd(); ++it )
    {
        dir.cd(*it);
        int num_files = dir.entryList(*(CPlayer::getInstance()->musicFilters()), QDir::Files).count();
        int num_dirs = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs).count();
        QStringList cover_list = dir.entryList(*(CPlayer::getInstance()->coverFilters()), QDir::Files);

        out->appendRow(new CTreeItem(*it, path + "/" + *it
                                    , (num_files ? QString("album") : QString("folder"))
                                    , cover_list.isEmpty() ? "" : dir.path() + "/" + cover_list.first()
                                    , level
                                    , num_files + num_dirs
                                    , this));
        dir.cdUp();
    }

    for( QStringList::const_iterator it = list_files.constBegin(); it != list_files.constEnd(); ++it )
    {
        out->appendRow(new CTreeItem(*it
                                    , path + "/" + *it
                                    , QString("file")
                                    , ""
                                    , level
                                    , 0
                                    , this));
    }

    return out;
}

QString CTree::parentDir(QString path)
{
    if( path == "" || path == "/" )
        return "";

    QDir dir = QDir(CPlayer::getInstance()->setting("ctree/root_music").toString() + path);
    dir.cdUp();

    return dir.path().replace(CPlayer::getInstance()->setting("ctree/root_music").toString(), "");
}

QString CTree::findCover(QString path)
{
    QDir dir = QDir(CPlayer::getInstance()->setting("ctree/root_music").toString() + path);
    QStringList cover_list = dir.entryList(*(CPlayer::getInstance()->coverFilters()), QDir::Files);

    return cover_list.isEmpty() ? "" : dir.path() + "/" + cover_list.first();
}

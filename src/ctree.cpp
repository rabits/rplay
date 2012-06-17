#include "ctree.h"
#include "cplayer.h"
#include "model/ckeyvalueitem.h"

CTree::CTree(QObject *parent)
    : QObject(parent)
{
    // Set music filters for fs
    m_music_filters << "*.wav" << "*.mp2" << "*.mp3" << "*.mp4" << "*.ogg" << "*.flac" << "*.aac";
}

QString CTree::findNextFile(QString path)
{
    QDir dir;
    path = libraryRoot(path);

    // Shuffle processing
    if( CPlayer::getInstance()->settingBool("preferences/shuffle_mode") )
    {
        if( m_shuffle_notplayed_list.empty() && CPlayer::getInstance()->settingBool("preferences/repeat_mode") )
            shuffleGenerateList();

        if( ! m_shuffle_notplayed_list.empty() )
        {
            qsrand(time(NULL));
            return libraryRootRemove(m_shuffle_notplayed_list.takeAt(qrand() % m_shuffle_notplayed_list.size()));
        }
        else
            return "";
    }

    // Sequential selection of next song
    dir = QFileInfo(path).absoluteDir();

    // Find in current directory
    QStringList list_files = dir.entryList(m_music_filters, QDir::Files);
    for( QStringList::const_iterator it = list_files.constBegin(); it != list_files.constEnd(); ++it )
    {
        QString fullpath = dir.absolutePath() + "/" + *it;
        if( fullpath == path )
        {
            if( (it+1) != list_files.constEnd() )
                return libraryRootRemove(dir.absolutePath() + "/" + *(it+1));
        }
    }

    while( libraryRootRemove(dir.absolutePath()) != "" )
    {
        if( libraryRootRemove(dir.absolutePath()) == CPlayer::getInstance()->setting("preferences/repeat_shuffle_folder").toString() )
            break;

        // Goto up
        QString previous_dir = dir.absolutePath();
        dir.cdUp();

        // Find next file in dirs upper then current
        QString output = findFirstFile(dir.absolutePath(), previous_dir);
        if( ! output.isEmpty() )
            return libraryRootRemove(output);
    }

    // Find first file in root - looping
    if( CPlayer::getInstance()->settingBool("preferences/repeat_mode") )
        return libraryRootRemove(findFirstFile(dir));

    return "";
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
    QStringList list_files = dir.entryList(m_music_filters, QDir::Files);
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
    QDir dir = QDir(libraryRoot(path));
    int level = path.split('/').count();

    QStringList list_dirs = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
    QStringList list_files = dir.entryList(m_music_filters, QDir::Files);

    for( QStringList::const_iterator it = list_dirs.constBegin(); it != list_dirs.constEnd(); ++it )
    {
        dir.cd(*it);
        int num_files = dir.entryList(m_music_filters, QDir::Files).count();
        int num_dirs = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs).count();

        out->appendRow(new CTreeItem(*it, path + "/" + *it
                                    , (num_files ? QString("album") : QString("folder"))
                                    , CPlayer::getInstance()->cover()->cover(path + "/" + *it)
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

    QDir dir = QDir(libraryRoot(path));

    return libraryRootRemove(dir.path().left(dir.path().length() - (dir.dirName().length() + 1)));
}

QString CTree::libraryRoot(QString path)
{
    return CPlayer::getInstance()->setting("preferences/music_library_path").toString() + path;
}

QString CTree::libraryRootRemove(QString path)
{
    return path.right(path.length() - CPlayer::getInstance()->setting("preferences/music_library_path").toString().length());
}

void CTree::shuffleGenerateList()
{
    qDebug("[rPlay] Generating shuffle list");
    QDir dir = libraryRoot(CPlayer::getInstance()->setting("preferences/repeat_shuffle_folder").toString());

    m_shuffle_notplayed_list = findFiles(QStringList() << dir.absolutePath());
}

QStringList CTree::findFiles(QStringList paths)
{
    QStringList result, more;
    QStringList::Iterator it;
    for( int i = 0; i < paths.size(); i++ )
    {
        QDir dir(paths[i]);

        // Files
        more = dir.entryList(m_music_filters, QDir::Files);
        for( it = more.begin(); it != more.end(); it++ )
            result.append(paths[i] + "/" + *it);

        // Subfolders
        more = dir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
        for( it = more.begin(); it != more.end(); it++ )
            *it = paths[i] + "/" + *it;
        more = findFiles(more);
        for( it = more.begin(); it != more.end(); it++ )
            result.append(*it);
    }

    return result;
}

#include "cplayer.h"
#include <QDeclarativeItem>

CPlayer::CPlayer(QObject *parent)
    : QObject(parent)
    , m_settings()
    , m_tree()
#if defined(MEEGO_EDITION_HARMATTAN)
    , m_hwkeys()
#endif
{
    // Set default settings
    if( m_settings.value("ctree/root_music").isNull() )
        m_settings.setValue("ctree/root_music", QString(QDir::homePath()));

    // Set filters for fs
    m_music_filters << "*.wav" << "*.mp3" << "*.ogg" << "*.flac";
    m_cover_filters << "cover.jpg" << "cover.jpeg" << "cover.png" << "folder.jpg" << "folder.jpeg" << "folder.png";

#if defined(MEEGO_EDITION_HARMATTAN)
    // Catch media keys events
    m_hwkeys = new MeeGo::QmKeys(this);
    connect(m_hwkeys, SIGNAL(keyEvent (MeeGo::QmKeys::Key, MeeGo::QmKeys::State)),
            this, SLOT(hwKeyEvent (MeeGo::QmKeys::Key, MeeGo::QmKeys::State)) );
#endif

    // Create player
    m_player = new QMediaPlayer(this, QMediaPlayer::LowLatency);
    connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));

    // Fulling metadata list
    m_metadata_list.insert(QtMultimediaKit::Title, "Title");
    m_metadata_list.insert(QtMultimediaKit::SubTitle, "Subtitle");
    m_metadata_list.insert(QtMultimediaKit::Author, "Author");
    m_metadata_list.insert(QtMultimediaKit::Comment, "Comment");
    m_metadata_list.insert(QtMultimediaKit::Description, "Description");
    m_metadata_list.insert(QtMultimediaKit::Category, "Category");
    m_metadata_list.insert(QtMultimediaKit::Genre, "Genre");
    m_metadata_list.insert(QtMultimediaKit::Year, "Year");
    m_metadata_list.insert(QtMultimediaKit::Date, "Date");
    m_metadata_list.insert(QtMultimediaKit::UserRating, "User Rating");
    m_metadata_list.insert(QtMultimediaKit::Keywords, "Keywords");
    m_metadata_list.insert(QtMultimediaKit::Language, "Language");
    m_metadata_list.insert(QtMultimediaKit::Publisher, "Publisher");
    m_metadata_list.insert(QtMultimediaKit::Copyright, "Copyright");
    m_metadata_list.insert(QtMultimediaKit::ParentalRating, "Parental Rating");
    m_metadata_list.insert(QtMultimediaKit::RatingOrganisation, "Rating Organisation");
    m_metadata_list.insert(QtMultimediaKit::AlbumTitle, "Album Title");
    m_metadata_list.insert(QtMultimediaKit::AlbumArtist, "Album Artist");
    m_metadata_list.insert(QtMultimediaKit::ContributingArtist, "Contributing Artist");
    m_metadata_list.insert(QtMultimediaKit::Composer, "Composer");
    m_metadata_list.insert(QtMultimediaKit::Conductor, "Conductor");
    m_metadata_list.insert(QtMultimediaKit::Lyrics, "Lyrics");
    m_metadata_list.insert(QtMultimediaKit::Mood, "Mood");
    m_metadata_list.insert(QtMultimediaKit::TrackNumber, "Track Number");
    m_metadata_list.insert(QtMultimediaKit::TrackCount, "Track Count");

    // Restore previous playing file
    if( ! setting("rplay/file").isNull() )
        m_player->setMedia(QUrl::fromLocalFile(setting("ctree/root_music").toString() + setting("rplay/file").toString()));
}

void CPlayer::initContext(QmlApplicationViewer& viewer)
{
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    m_context = viewer.rootContext();

    m_context->setContextProperty("cplayer", this);
    m_context->setContextProperty("ctree", tree());
    m_context->setContextProperty("current_file", currentFile());
    m_context->setContextProperty("current_file_array", currentFileArray());
}

void CPlayer::initRoot(QmlApplicationViewer& viewer)
{
    m_root_object = viewer.rootObject();
}

QVariant CPlayer::setting(QString key, QString value)
{
    if( ! value.isEmpty() )
        m_settings.setValue(key, value);

    return m_settings.value(key);
}

QHash<QString, QVariant> CPlayer::getMetaData()
{
    qDebug("Start find metadata");
    QHash<QString, QVariant> out;

    QList<QtMultimediaKit::MetaData> mdlist = m_player->availableMetaData();
    int mdsize = mdlist.size();

    QtMultimediaKit::MetaData key;

    for( int i = 0; i < mdsize; i++ )
    {
        key = mdlist.at(i);
        if( m_metadata_list[key].isEmpty() )
            out.insert(QString(key), m_player->metaData(key));
        else
            out.insert(m_metadata_list[key], m_player->metaData(key));

        qDebug(m_metadata_list[key].toStdString().c_str());
        qDebug(m_player->metaData(key).toString().toStdString().c_str());
    }

    return out;
}

QHash<QString, QVariant> CPlayer::getExtendedMetaData()
{
    qDebug("Start find ext metadata");
    QHash<QString, QVariant> out;

    QStringList emdlist = m_player->availableExtendedMetaData();

    for( QStringList::const_iterator it = emdlist.constBegin(); it != emdlist.constEnd(); ++it )
    {
        out.insert(*it, m_player->extendedMetaData(*it));
        qDebug((*it).toStdString().c_str());
        qDebug(m_player->extendedMetaData(*it).toString().toStdString().c_str());
    }

    return out;
}

void CPlayer::statusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status) {
    case QMediaPlayer::EndOfMedia:
        playNext();
        break;
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadingMedia:
    case QMediaPlayer::LoadedMedia:
    case QMediaPlayer::StalledMedia:
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
    case QMediaPlayer::InvalidMedia:
        break;
    }
}

void CPlayer::playFile(QString path)
{
    if( setting("rplay/file").toString() != path )
    {
        setting("rplay/file", path);
        m_player->setMedia(QUrl::fromLocalFile(setting("ctree/root_music").toString() + setting("rplay/file").toString()));
        play();

        qDebug("Playing file:");
        qDebug(setting("rplay/file").toString().toStdString().c_str());
    }
    else
    {
        if( m_player->state() == QMediaPlayer::PlayingState )
            pause();
        else
            play();
    }

    m_context->setContextProperty("current_file_array", currentFileArray());
}

void CPlayer::playNext()
{
    playFile(m_tree.findNextFile(setting("rplay/file").toString()));
}

void CPlayer::playPrev()
{
}

void CPlayer::play()
{
    setting("rplay/state", "playing");

    m_player->play();
}

void CPlayer::pause()
{
    setting("rplay/state", "pause");

    m_player->pause();
}

#if defined(MEEGO_EDITION_HARMATTAN)
void CPlayer::hwKeyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state)
{
    if( state == MeeGo::QmKeys::KeyUp )
    {
        qDebug("!key released!");
        switch (key) {
        case MeeGo::QmKeys::NextSong :
            playNext();
            break;
        case MeeGo::QmKeys::PlayPause :
            if( m_player->state() == QMediaPlayer::PlayingState )
                pause();
            else
                play();
            break;
        case MeeGo::QmKeys::Play :
            play();
            break;
        case MeeGo::QmKeys::Pause :
            pause();
            break;
        case MeeGo::QmKeys::UnknownKey :
        case MeeGo::QmKeys::KeyboardSlider :
        case MeeGo::QmKeys::Camera :
        case MeeGo::QmKeys::VolumeUp :
        case MeeGo::QmKeys::VolumeDown :
        case MeeGo::QmKeys::Phone :
        case MeeGo::QmKeys::Stop :
        case MeeGo::QmKeys::Forward :
        case MeeGo::QmKeys::Rewind :
        case MeeGo::QmKeys::Mute :
        case MeeGo::QmKeys::LeftKey :
        case MeeGo::QmKeys::RightKey :
        case MeeGo::QmKeys::UpKey :
        case MeeGo::QmKeys::DownKey :
        case MeeGo::QmKeys::End :
        case MeeGo::QmKeys::PreviousSong :
        case MeeGo::QmKeys::RightCtrl :
        case MeeGo::QmKeys::PowerKey :
            break;
        }
    }
}
#endif

CPlayer* CPlayer::s_pInstance = NULL;

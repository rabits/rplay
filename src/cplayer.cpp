#include "cplayer.h"
#include <QDeclarativeItem>
#include "model/ckeyvalueitem.h"

CPlayer::CPlayer(QObject *parent)
    : QObject(parent)
    , m_settings()
    , m_tree()
#if defined(MEEGO_EDITION_HARMATTAN)
    , m_hwkeys()
#endif
{
    // Set default settings
    if( m_settings.value("preferences/music_library_path").isNull() )
        m_settings.setValue("preferences/music_library_path", QString(QDir::homePath()));
    if( m_settings.value("preferences/lyrics_extension").isNull() )
        m_settings.setValue("preferences/lyrics_extension", "txt");

    // Set filters for fs
    m_music_filters << "*.wav" << "*.mp2" << "*.mp3" << "*.mp4" << "*.ogg" << "*.flac" << "*.aac";
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
    connect(m_player, SIGNAL(metaDataChanged()), this, SIGNAL(metaDataChanged()));

    // Fulling metadata list
    m_metadata_list.insert(QtMultimediaKit::AlbumArtist, "Artist");
    m_metadata_list.insert(QtMultimediaKit::AlbumTitle, "Album");
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
    m_metadata_list.insert(QtMultimediaKit::ContributingArtist, "Contributing Artist");
    m_metadata_list.insert(QtMultimediaKit::Composer, "Composer");
    m_metadata_list.insert(QtMultimediaKit::Conductor, "Conductor");
    m_metadata_list.insert(QtMultimediaKit::Lyrics, "Lyrics");
    m_metadata_list.insert(QtMultimediaKit::Mood, "Mood");
    m_metadata_list.insert(QtMultimediaKit::TrackNumber, "Track Number");
    m_metadata_list.insert(QtMultimediaKit::TrackCount, "Track Count");
    m_metadata_list.insert(QtMultimediaKit::Size, "Size");
    m_metadata_list.insert(QtMultimediaKit::MediaType, "Type");
    m_metadata_list.insert(QtMultimediaKit::Duration, "Duration");
    m_metadata_list.insert(QtMultimediaKit::AudioBitRate, "Audio Bit Rate");
    m_metadata_list.insert(QtMultimediaKit::AudioCodec, "Audio Codec");
    m_metadata_list.insert(QtMultimediaKit::AverageLevel, "Average Level");
    m_metadata_list.insert(QtMultimediaKit::ChannelCount, "Channels");
    m_metadata_list.insert(QtMultimediaKit::PeakValue, "Peak");
    m_metadata_list.insert(QtMultimediaKit::SampleRate, "Sample Rate");

    // Set ordering
    m_metadata_list_order.append(QtMultimediaKit::AlbumArtist);
    m_metadata_list_order.append(QtMultimediaKit::AlbumTitle);
    m_metadata_list_order.append(QtMultimediaKit::Title);
    m_metadata_list_order.append(QtMultimediaKit::Year);
    m_metadata_list_order.append(QtMultimediaKit::Genre);
    m_metadata_list_order.append(QtMultimediaKit::SubTitle);
    m_metadata_list_order.append(QtMultimediaKit::Author);
    m_metadata_list_order.append(QtMultimediaKit::Comment);
    m_metadata_list_order.append(QtMultimediaKit::Description);
    m_metadata_list_order.append(QtMultimediaKit::Category);
    m_metadata_list_order.append(QtMultimediaKit::Date);
    m_metadata_list_order.append(QtMultimediaKit::UserRating);
    m_metadata_list_order.append(QtMultimediaKit::Keywords);
    m_metadata_list_order.append(QtMultimediaKit::Language);
    m_metadata_list_order.append(QtMultimediaKit::Publisher);
    m_metadata_list_order.append(QtMultimediaKit::Copyright);
    m_metadata_list_order.append(QtMultimediaKit::ParentalRating);
    m_metadata_list_order.append(QtMultimediaKit::RatingOrganisation);
    m_metadata_list_order.append(QtMultimediaKit::ContributingArtist);
    m_metadata_list_order.append(QtMultimediaKit::Composer);
    m_metadata_list_order.append(QtMultimediaKit::Conductor);
    m_metadata_list_order.append(QtMultimediaKit::Lyrics);
    m_metadata_list_order.append(QtMultimediaKit::Mood);
    m_metadata_list_order.append(QtMultimediaKit::TrackNumber);
    m_metadata_list_order.append(QtMultimediaKit::TrackCount);
    m_metadata_list_order.append(QtMultimediaKit::Size);
    m_metadata_list_order.append(QtMultimediaKit::MediaType);
    m_metadata_list_order.append(QtMultimediaKit::Duration);
    m_metadata_list_order.append(QtMultimediaKit::AudioBitRate);
    m_metadata_list_order.append(QtMultimediaKit::AudioCodec);
    m_metadata_list_order.append(QtMultimediaKit::AverageLevel);
    m_metadata_list_order.append(QtMultimediaKit::ChannelCount);
    m_metadata_list_order.append(QtMultimediaKit::PeakValue);
    m_metadata_list_order.append(QtMultimediaKit::SampleRate);

    // Restore previous playing file
    if( ! setting("rplay/file").isNull() )
        m_player->setMedia(QUrl::fromLocalFile(setting("preferences/music_library_path").toString() + setting("rplay/file").toString()));
}

void CPlayer::initContext(QmlApplicationViewer& viewer)
{
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    m_context = viewer.rootContext();

    m_context->setContextProperty("cplayer", this);
    m_context->setContextProperty("ctree", tree());
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

ListModel *CPlayer::getMetaData()
{
    ListModel *out = new ListModel(new CKeyValueItem(), parent());

    if( m_player->isMetaDataAvailable() )
    {
        QList<QtMultimediaKit::MetaData> mdlist = m_player->availableMetaData();
        QHash<QtMultimediaKit::MetaData, QString> duplist;

        QString value;

        for( QList<QtMultimediaKit::MetaData>::const_iterator it = m_metadata_list_order.constBegin(); it != m_metadata_list_order.constEnd(); it++ )
        {
            if( mdlist.contains(*it) )
            {
                value = m_player->metaData(*it).toString();
                if( ! (value.isEmpty() || duplist.value(*it, "") == value) )
                {
                    out->appendRow(new CKeyValueItem(QString::number(*it), m_metadata_list[*it], m_player->metaData(*it).toString(), "meta", this));
                    duplist.insert(*it, value);
                }
            }
        }
    }

    return out;
}

ListModel *CPlayer::getExtendedMetaData()
{
    ListModel *out = new ListModel(new CKeyValueItem(), parent());

    if( m_player->isMetaDataAvailable() )
    {
        QMap<QString, QString> duplist;
        QString value;

        QStringList emdlist = m_player->availableExtendedMetaData();
        emdlist.sort();

        for( QStringList::const_iterator it = emdlist.constBegin(); it != emdlist.constEnd(); ++it )
        {
            value = m_player->extendedMetaData(*it).toString();
            if( ! (value.isEmpty() || duplist.value(*it, "") == value) )
            {
                out->appendRow(new CKeyValueItem(*it, *it, m_player->extendedMetaData(*it).toString(), "extmeta", this));
                duplist.insert(*it, value);
            }
        }
    }

    return out;
}

ListModel *CPlayer::getLyrics(QString path)
{
    if( path == "" )
        path = currentFile();
    path = setting("preferences/music_library_path").toString() + path;
    path = path.left(path.length() - QFileInfo(path).suffix().length()).append(setting("preferences/lyrics_extension").toString());

    ListModel *out = new ListModel(new CKeyValueItem(), parent());

    QFile lyrics(path);

    // If file less then 50Kb and readable
    if( (lyrics.size() < 50 * 1024) && lyrics.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&lyrics);
        out->appendRow(new CKeyValueItem(path, "Lyrics", in.readAll(), "text", this));
    }

    return out;
}

ListModel *CPlayer::prefsContent()
{
    ListModel* out = new ListModel(new CKeyValueItem(), parent());

    out->appendRow(new CKeyValueItem("preferences/music_library_path", "Music Library path"
                                     , setting("preferences/music_library_path").toString()
                                     , "folder_path", this));

    QStringList about;
    about << "Program: rPlay v1.0.0" << "Author:  Rabit <home.rabits@gmail.com>" << "Site:    https://github.com/rabits/rplay";

    out->appendRow(new CKeyValueItem("", "About"
                                     , about.join("\n")
                                     , "about", this));

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
        m_player->setMedia(QUrl::fromLocalFile(setting("preferences/music_library_path").toString() + setting("rplay/file").toString()));
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
    emit next();
}

void CPlayer::playRev()
{
    m_player->setPosition(0);
}

void CPlayer::play()
{
    m_player->play();
}

void CPlayer::pause()
{
    m_player->pause();
}

#if defined(MEEGO_EDITION_HARMATTAN)
void CPlayer::hwKeyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state)
{
    if( state == MeeGo::QmKeys::KeyUp )
    {
        switch (key) {
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
        case MeeGo::QmKeys::NextSong :
            playNext();
            break;
        case MeeGo::QmKeys::PreviousSong :
            playRev();
            break;
        case MeeGo::QmKeys::UnknownKey :
            qDebug("UnknownKey");
            break;
        case MeeGo::QmKeys::KeyboardSlider :
            qDebug("KeyboardSlider");
            break;
        case MeeGo::QmKeys::Camera :
            qDebug("Camera");
            break;
        case MeeGo::QmKeys::VolumeUp :
            qDebug("VolumeUp");
            break;
        case MeeGo::QmKeys::VolumeDown :
            qDebug("VolumeDown");
            break;
        case MeeGo::QmKeys::Phone :
            qDebug("Phone");
            break;
        case MeeGo::QmKeys::Stop :
            qDebug("Stop");
            break;
        case MeeGo::QmKeys::Forward :
            qDebug("Forward");
            break;
        case MeeGo::QmKeys::Rewind :
            qDebug("Rewind");
            break;
        case MeeGo::QmKeys::Mute :
            qDebug("Mute");
            break;
        case MeeGo::QmKeys::LeftKey :
            qDebug("LeftKey");
            break;
        case MeeGo::QmKeys::RightKey :
            qDebug("RightKey");
            break;
        case MeeGo::QmKeys::UpKey :
            qDebug("UpKey");
            break;
        case MeeGo::QmKeys::DownKey :
            qDebug("DownKey");
            break;
        case MeeGo::QmKeys::End :
            qDebug("End");
            break;
        case MeeGo::QmKeys::RightCtrl :
            qDebug("RightCtrl");
            break;
        case MeeGo::QmKeys::PowerKey :
            qDebug("PowerKey");
            break;
        }
    }
}
#endif

CPlayer* CPlayer::s_pInstance = NULL;

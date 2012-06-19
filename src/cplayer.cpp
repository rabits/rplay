#include "cplayer.h"
#include <QDeclarativeItem>
#include "model/ckeyvalueitem.h"

CPlayer::CPlayer(QObject *parent)
    : QObject(parent)
    , m_settings()
    , m_tree()
#ifdef USE_VOICE
    , m_voice()
#endif
    , m_lyrics()
    , m_cover()
#if defined(MEEGO_EDITION_HARMATTAN)
    , m_hwkeys(NULL)
#endif
    , m_translator()
{
    qDebug("[rPlay] Init player");
    // Set default settings
    if( m_settings.value("preferences/music_library_path").isNull() )
        m_settings.setValue("preferences/music_library_path", QString(QDir::homePath()));

    if( m_settings.value("preferences/locale").isNull() )
        m_settings.setValue("preferences/locale", QLocale::system().name());

    if( m_settings.value("preferences/lyrics_extension").isNull() )
        m_settings.setValue("preferences/lyrics_extension", "txt");

    if( m_settings.value("preferences/repeat_mode").isNull() )
        m_settings.setValue("preferences/repeat_mode", true);

    if( m_settings.value("preferences/shuffle_mode").isNull() )
        m_settings.setValue("preferences/shuffle_mode", false);

    if( m_settings.value("preferences/repeat_shuffle_folder").isNull() )
        m_settings.setValue("preferences/repeat_shuffle_folder", "/");

    if( m_settings.value("preferences/network_autoconnect").isNull() )
        m_settings.setValue("preferences/network_autoconnect", false);

    if( m_settings.value("preferences/network_get_lyrics").isNull() )
        m_settings.setValue("preferences/network_get_lyrics", true);
    if( m_settings.value("preferences/network_get_lyrics_show_always").isNull() )
        m_settings.setValue("preferences/network_get_lyrics_show_always", false);
    if( m_settings.value("preferences/network_get_lyrics_only_on_songlyrics").isNull() )
        m_settings.setValue("preferences/network_get_lyrics_only_on_songlyrics", true);
    if( m_settings.value("preferences/network_get_lyrics_autosave").isNull() )
        m_settings.setValue("preferences/network_get_lyrics_autosave", false);

    if( m_settings.value("preferences/network_get_covers").isNull() )
        m_settings.setValue("preferences/network_get_covers", true);

#ifdef USE_VOICE
    if( m_settings.value("preferences/voice_say_on_meta_changed").isNull() )
        m_settings.setValue("preferences/voice_say_on_meta_changed", true);
    if( m_settings.value("preferences/voice_artist_album_say").isNull() )
        m_settings.setValue("preferences/voice_artist_album_say", true);
    if( m_settings.value("preferences/voice_title_say").isNull() )
        m_settings.setValue("preferences/voice_title_say", false);
#endif

    if( m_settings.value("rplay/show_help_once").isNull() )
        m_settings.setValue("rplay/show_help_once", true);

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
    connect(this, SIGNAL(mediaFullyLoaded()), cover(), SLOT(coverNetCurrent()));

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

CPlayer::~CPlayer()
{
    delete m_player;
    delete m_hwkeys;
}

void CPlayer::initContext(QmlApplicationViewer& viewer, QScopedPointer<QApplication> *app)
{
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    m_context = viewer.rootContext();

    m_context->setContextProperty("cplayer", this);
    m_context->setContextProperty("ctree", &m_tree);
#ifdef USE_VOICE
    m_context->setContextProperty("cvoice", &m_voice);
#endif
    m_context->setContextProperty("clyrics", &m_lyrics);
    m_context->setContextProperty("ccover", &m_cover);
    m_context->setContextProperty("current_file_array", currentFileArray());

    // Init shuffle
    if( settingBool("preferences/shuffle_mode") )
        m_tree.shuffleGenerateList();

#ifdef USE_VOICE
    // Voice engine connection
    connect(this, SIGNAL(mediaFullyLoaded()), &m_voice, SLOT(sayCurrent()));
#endif

    m_app = app;
    changeLocale(setting("preferences/locale").toString());
    (*m_app)->installTranslator(&m_translator);
}

void CPlayer::changeLocale(QString locale)
{
    qDebug(("[rPlay] changing locale to " + locale).toStdString().c_str());
    if( ! m_translator.load("tr_" + locale, ":/") )
    {
        m_translator.load("tr_en", ":/");
        setting("preferences/locale", "en");
    }
}

void CPlayer::initRoot(QmlApplicationViewer& viewer)
{
    m_root_object = viewer.rootObject();
}

QVariant CPlayer::setting(QString key, QString value)
{
    if( ! value.isEmpty() )
    {
        m_settings.setValue(key, value);
        if( key.contains(QString("preferences/")) )
            emit settingChanged(key);
    }

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

ListModel *CPlayer::prefsContent()
{
    ListModel* out = new ListModel(new CKeyValueItem(), parent());

    out->appendRow(new CKeyValueItem("preferences/music_library_path", tr("Music Library path")
                                     , setting("preferences/music_library_path").toString()
                                     , "folder_path", this));

    out->appendRow(new CKeyValueItem("preferences/repeat_mode", tr("Repeat mode")
                                     , setting("preferences/repeat_mode").toString()
                                     , "bool", this));

    out->appendRow(new CKeyValueItem("preferences/shuffle_mode", tr("Shuffle mode")
                                     , setting("preferences/shuffle_mode").toString()
                                     , "bool", this));

    out->appendRow(new CKeyValueItem("preferences/repeat_shuffle_folder", tr("Repeat and Shuffle folder")
                                     , setting("preferences/repeat_shuffle_folder").toString()
                                     , "readonly", this));

#ifdef USE_VOICE
    out->appendRow(new CKeyValueItem("preferences/voice_say_on_meta_changed", tr("Enable Voice")
                                     , setting("preferences/voice_say_on_meta_changed").toString()
                                     , "bool", this));
    out->appendRow(new CKeyValueItem("preferences/voice_artist_album_say", tr("Voice say artist and album")
                                     , setting("preferences/voice_artist_album_say").toString()
                                     , "bool", this));
    out->appendRow(new CKeyValueItem("preferences/voice_title_say", tr("Voice say title")
                                     , setting("preferences/voice_title_say").toString()
                                     , "bool", this));
#endif

    out->appendRow(new CKeyValueItem("preferences/network_autoconnect", tr("Automatic request network connection")
                                     , setting("preferences/network_autoconnect").toString()
                                     , "bool", this));

    out->appendRow(new CKeyValueItem("preferences/network_get_lyrics", tr("Download lyrics from lyrdb")
                                     , setting("preferences/network_get_lyrics").toString()
                                     , "bool", this));

    out->appendRow(new CKeyValueItem("preferences/network_get_lyrics_show_always", tr("Show downloaded lyrics if present local lyrics")
                                     , setting("preferences/network_get_lyrics_show_always").toString()
                                     , "bool", this));

    out->appendRow(new CKeyValueItem("preferences/network_get_lyrics_only_on_songlyrics", tr("Lyrics downloading only if on SongPage opened lyrics")
                                     , setting("preferences/network_get_lyrics_only_on_songlyrics").toString()
                                     , "bool", this));

    out->appendRow(new CKeyValueItem("preferences/network_get_lyrics_autosave", tr("Autosave first downloaded lyrics if local not present")
                                     , setting("preferences/network_get_lyrics_autosave").toString()
                                     , "bool", this));

    out->appendRow(new CKeyValueItem("preferences/network_get_covers", tr("Download covers from lastfm and autosave it")
                                     , setting("preferences/network_get_covers").toString()
                                     , "bool", this));

    QStringList about;
    about << tr("Program: rPlay v%1").arg(PROJECT_VERSION)
          << tr("Author:  %1").arg("Rabit <home.rabits@gmail.com>")
          << tr("Site:    %1").arg("https://github.com/rabits/rplay");

    out->appendRow(new CKeyValueItem("", tr("About (click to see help)")
                                     , about.join("\n")
                                     , "about", this));

    return out;
}

QString CPlayer::helpContent()
{
    QStringList help;
    help << tr("<b>rPlay</b> - is minimalistic audio player with orientation to filesystem albums grouping.")
         << tr("<b>Basic features</b>: simple interface, filesystem-oriented, looping and shuffle, display album covers and lyrics with downloading it from net, id3 tag supports, headset support, voice notification of current track.")
         << tr("<b>Supported formats</b>: %1").arg("wav, mp2, mp3, mp4, ogg, flac, aac.")
         << ""
         << tr("<b>Some help</b>:")
         << tr("Player contains 3 pages: SettingsPage, TreePage and SongPage. Every page contains Header and List.")
         << tr("Common:")
         << tr(" - To change page - simple scroll left-right on header info.")
         << tr("TreePage:")
         << tr(" - Navigation - click on header to go in parent folder; click to folder in list to go into that folder.")
         << tr(" - Play/Pause - click on track in list.")
         << tr(" - Set repeat/shuffle folder - go into need folder and long press on header.")
         << tr("SettingsPage:")
         << tr(" - Change setting - click to item in list.")
         << tr("SongPage:")
         << tr(" - Play/Pause - click to cover in header.")
         << tr(" - Change current track position - click to scrollbar in header.")
         << tr(" - Change view info (Metadata, Ext Metadata and Lyrics) - click to header info.")
         << tr(" - Change lyric - click to lyric in list.")
         << ""
         << tr("Good luck in the use. All issues, suggestions and comments you can send on %1 or %2")
            .arg("<a href=\"mailto:home.rabits@gmail.com\">%3</a>").arg("<a href=\"https://github.com/rabits/rplay/issues\">%4</a>")
            .arg(tr("e-mail")).arg(tr("issues site"));
    return help.join("<br/>\n");
}

void CPlayer::statusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status) {
    case QMediaPlayer::EndOfMedia:
        playNext();
        break;
    case QMediaPlayer::UnknownMediaStatus:
        qDebug("[rPlay] Media unknown");
        break;
    case QMediaPlayer::NoMedia:
        qDebug("[rPlay] Media no");
        break;
    case QMediaPlayer::LoadingMedia:
        qDebug("[rPlay] Media loading");
        break;
    case QMediaPlayer::LoadedMedia:
        qDebug("[rPlay] Media loaded");
        break;
    case QMediaPlayer::StalledMedia:
        qDebug("[rPlay] Media stalled");
        break;
    case QMediaPlayer::BufferingMedia:
        qDebug("[rPlay] Media buffering");
        break;
    case QMediaPlayer::BufferedMedia:
        qDebug("[rPlay] Media buffered");
        emit mediaFullyLoaded();
        break;
    case QMediaPlayer::InvalidMedia:
        qDebug("[rPlay] Media invalid");
        break;
    }
}

void CPlayer::playFile(QString path)
{
    if( setting("rplay/file").toString() != path )
    {
        if( path != "" )
        {
            m_lyrics.downloadedLyricsClear();
            setting("rplay/file", path);
            m_player->setMedia(QUrl::fromLocalFile(tree()->libraryRoot(path)));
            play();
            emit newTrack();

            qDebug(QString("[rPlay] Playing file: ").append(setting("rplay/file").toString()).toStdString().c_str());
        }
        else
        {
            m_player->stop();
            qDebug("[rPlay] Stop playing");
        }
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
    emit nextTrack();
}

void CPlayer::playRew()
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
            playRew();
            break;
        case MeeGo::QmKeys::UnknownKey :
            qDebug("[rPlay] UnknownKey");
            break;
        case MeeGo::QmKeys::KeyboardSlider :
            qDebug("[rPlay] KeyboardSlider");
            break;
        case MeeGo::QmKeys::Camera :
            qDebug("[rPlay] Camera");
            break;
        case MeeGo::QmKeys::VolumeUp :
            qDebug("[rPlay] VolumeUp");
            break;
        case MeeGo::QmKeys::VolumeDown :
            qDebug("[rPlay] VolumeDown");
            break;
        case MeeGo::QmKeys::Phone :
            qDebug("[rPlay] Phone");
            break;
        case MeeGo::QmKeys::Stop :
            qDebug("[rPlay] Stop");
            break;
        case MeeGo::QmKeys::Forward :
            qDebug("[rPlay] Forward");
            break;
        case MeeGo::QmKeys::Rewind :
            qDebug("[rPlay] Rewind");
            break;
        case MeeGo::QmKeys::Mute :
            qDebug("[rPlay] Mute");
            break;
        case MeeGo::QmKeys::LeftKey :
            qDebug("[rPlay] LeftKey");
            break;
        case MeeGo::QmKeys::RightKey :
            qDebug("[rPlay] RightKey");
            break;
        case MeeGo::QmKeys::UpKey :
            qDebug("[rPlay] UpKey");
            break;
        case MeeGo::QmKeys::DownKey :
            qDebug("[rPlay] DownKey");
            break;
        case MeeGo::QmKeys::End :
            qDebug("[rPlay] End");
            break;
        case MeeGo::QmKeys::RightCtrl :
            qDebug("[rPlay] RightCtrl");
            break;
        case MeeGo::QmKeys::PowerKey :
            qDebug("[rPlay] PowerKey");
            break;
        }
    }
}
#endif

CPlayer* CPlayer::s_pInstance = NULL;

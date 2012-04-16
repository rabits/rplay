#include "cplayer.h"
#include <QDeclarativeItem>

CPlayer::CPlayer(QObject *parent)
    : QObject(parent)
    , m_settings()
    , m_tree()
    , m_hwkeys()
{
    // Set default settings
    if( m_settings.value("ctree/root_music").isNull() )
        m_settings.setValue("ctree/root_music", QString(QDir::homePath()));

    // Set filters for fs
    m_music_filters << "*.wav" << "*.mp3" << "*.ogg" << "*.flac";
    m_cover_filters << "cover.jpg" << "cover.jpeg" << "cover.png" << "folder.jpg" << "folder.jpeg" << "folder.png";

    // Catch media keys events
    m_hwkeys = new MeeGo::QmKeys(this);
    connect(m_hwkeys, SIGNAL(keyEvent (MeeGo::QmKeys::Key, MeeGo::QmKeys::State)),
            this, SLOT(hwKeyEvent (MeeGo::QmKeys::Key, MeeGo::QmKeys::State)) );

    // Create player
    m_player = new QMediaPlayer(this);
    connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));

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

CPlayer* CPlayer::s_pInstance = NULL;

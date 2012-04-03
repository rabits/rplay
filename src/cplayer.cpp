#include "cplayer.h"

CPlayer::CPlayer(QObject *parent)
    : QObject(parent)
    , m_settings()
    , m_tree()
{
    // Set default settings
    if( m_settings.value("ctree/root_music").isNull() )
        m_settings.setValue("ctree/root_music", QString(QDir::homePath()));

    // Set filters for fs
    m_music_filters << "*.wav" << "*.mp3" << "*.ogg" << "*.flac";
    m_cover_filters << "cover.jpg" << "cover.jpeg" << "cover.png" << "folder.jpg" << "folder.jpeg" << "folder.png";

    // Create player
    m_player = new QMediaPlayer(this);
    connect(m_player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
}

void CPlayer::initContext(QmlApplicationViewer& viewer)
{
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    m_context = viewer.rootContext();
    m_context->setContextProperty("cplayer", this);
    m_context->setContextProperty("ctree", tree());
    m_context->setContextProperty("current_file", currentFile());
    m_context->setContextProperty("current_state", currentState());
}

QVariant CPlayer::setting(QString key, QString value)
{
    qDebug("get value");
    if( ! value.isEmpty() )
    {
        qDebug("value changed");
        m_settings.setValue(key, value);
    }

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
        if( setting("rplay/state").toString() == "playing" )
            pause();
        else
            play();
    }
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

CPlayer* CPlayer::s_pInstance = NULL;

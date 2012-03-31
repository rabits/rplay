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
    m_current_file = CPlayer::getInstance()->setting("ctree/root_music").toString() + path;

    m_player->setMedia(QUrl::fromLocalFile(m_current_file));
    m_player->play();

    qDebug("Playing file:");
    qDebug(m_current_file.toStdString().c_str());
}

void CPlayer::playNext()
{
    playFile(m_tree.findNextFile(m_current_file).replace(CPlayer::getInstance()->setting("ctree/root_music").toString(), QString()));
}

void CPlayer::playPrev()
{
}

CPlayer* CPlayer::s_pInstance = NULL;

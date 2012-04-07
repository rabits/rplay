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
    // Restore previous state
    if( ! setting("rplay/file").isNull() )
        m_player->setMedia(QUrl::fromLocalFile(setting("ctree/root_music").toString() + setting("rplay/file").toString()));
    if( setting("rplay/state").toString() == "playing" )
        play();
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
        if( setting("rplay/state").toString() == "playing" )
            pause();
        else
            play();
    }

    QObject* root_list = m_root_object->findChild<QObject *>("rootList");
    QDeclarativeItem* content_item = qvariant_cast<QDeclarativeItem*>(root_list->property("contentItem"));

    foreach( QGraphicsItem* item, content_item->childItems() )
    {
        if( qobject_cast<QDeclarativeItem*>(item)->objectName() == "/group" )
            qDebug("Found /group");
    }
/*
    QObject* item = items.first();
    QVariant returned_value;
    if( item )
        QMetaObject::invokeMethod(item, "select", Q_RETURN_ARG(QVariant, returned_value));
    else
    {
        qDebug("Object not found");
        qDebug(path.toStdString().c_str());
    }
    */
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
            if( setting("rplay/state").toString() == "playing" )
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
        }
    }
}

CPlayer* CPlayer::s_pInstance = NULL;

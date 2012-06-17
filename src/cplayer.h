#ifndef CPLAYER_H
#define CPLAYER_H

#include <QObject>
#include <QSettings>
#include <QtDeclarative/QDeclarativeContext>
#include <QGraphicsObject>
#include <QtMultimediaKit/qmediaplayer.h>
#include <QTranslator>
#include <QApplication>

#include <qplatformdefs.h> // MEEGO_EDITION_HARMATTAN

#if defined(MEEGO_EDITION_HARMATTAN)
    #include <qmsystem2/qmkeys.h>
#endif

#include "qmlapplicationviewer.h"

#include "src/ctree.h"
#ifdef USE_VOICE
    #include "src/cvoice.h"
#endif
#include "clyrics.h"
#include "ccover.h"

class CPlayer : public QObject
{
    Q_OBJECT
private:
    explicit CPlayer(QObject *parent = 0);
    ~CPlayer();

    static CPlayer* s_pInstance;

    QGraphicsObject*     m_root_object;
    QDeclarativeContext* m_context;

    QSettings   m_settings;
    CTree       m_tree;
#ifdef USE_VOICE
    CVoice      m_voice;
#endif
    CLyrics     m_lyrics;
    CCover      m_cover;

#if defined(MEEGO_EDITION_HARMATTAN)
    MeeGo::QmKeys* m_hwkeys;
#endif

    QMediaPlayer*        m_player;

    QHash<QtMultimediaKit::MetaData, QString> m_metadata_list;
    QList<QtMultimediaKit::MetaData> m_metadata_list_order;

    QTranslator                   m_translator;
    QScopedPointer<QApplication> *m_app;

public:
    inline static CPlayer* getInstance() { if( s_pInstance == NULL ) s_pInstance = new CPlayer(); return s_pInstance; }
    inline static void     destroyInstance() { delete s_pInstance; }

    void initContext(QmlApplicationViewer& viewer, QScopedPointer<QApplication> *app);
    void initRoot(QmlApplicationViewer& viewer);

    Q_INVOKABLE inline CTree*   tree() { return &m_tree; }
    Q_INVOKABLE inline CLyrics* lyrics() { return &m_lyrics; }
    Q_INVOKABLE inline CCover*  cover() { return &m_cover; }
    Q_INVOKABLE void changeLocale(QString locale);

    Q_INVOKABLE inline QSettings* settings() { return &m_settings; }
    Q_INVOKABLE QVariant setting(QString key, QString value = "");
    Q_INVOKABLE inline bool settingBool(QString key) { return m_settings.value(key).toBool(); }

    Q_INVOKABLE inline QString currentFile() { return CPlayer::getInstance()->setting("rplay/file").toString(); }
    Q_INVOKABLE inline QStringList currentFileArray() { return CPlayer::getInstance()->setting("rplay/file").toString().split('/'); }
    Q_INVOKABLE inline qint64 currentFilePosition() { return m_player->position(); }
    Q_INVOKABLE inline void currentFilePosition(qint64 pos) { m_player->setPosition(pos); }
    Q_INVOKABLE inline qint64 currentFileDuration() { return m_player->duration(); }

    Q_INVOKABLE inline bool fullyLoaded() { return m_player->mediaStatus() == QMediaPlayer::LoadedMedia; }

    Q_INVOKABLE ListModel* getMetaData();
    Q_INVOKABLE ListModel* getExtendedMetaData();

    Q_INVOKABLE ListModel* prefsContent();

    Q_INVOKABLE QString helpContent();

    Q_INVOKABLE void playFile(QString path);
    Q_INVOKABLE void playNext();
    Q_INVOKABLE void playRew();

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();

    Q_INVOKABLE int  volume() { return m_player->volume(); }
    Q_INVOKABLE void volume(int val) { m_player->setVolume(val); }

    Q_INVOKABLE QString artist() { return m_player->metaData(QtMultimediaKit::AlbumArtist).toString(); }
    Q_INVOKABLE QString album() { return m_player->metaData(QtMultimediaKit::AlbumTitle).toString(); }
    Q_INVOKABLE QString title() { return m_player->metaData(QtMultimediaKit::Title).toString(); }

signals:
    void newTrack();
    void nextTrack();
    void settingChanged(QString key);
    void metaDataChanged();
    void mediaFullyLoaded();

public slots:
    void statusChanged(QMediaPlayer::MediaStatus status);
#if defined(MEEGO_EDITION_HARMATTAN)
    void hwKeyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state);
#endif
};

#endif // CPLAYER_H

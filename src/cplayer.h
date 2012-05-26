#ifndef CPLAYER_H
#define CPLAYER_H

#include <QObject>
#include <QSettings>
#include <QtDeclarative/QDeclarativeContext>
#include <QGraphicsObject>
#include <QtMultimediaKit/qmediaplayer.h>

#include <qplatformdefs.h> // MEEGO_EDITION_HARMATTAN

#if defined(MEEGO_EDITION_HARMATTAN)
    #include <qmsystem2/qmkeys.h>
#endif

#include "qmlapplicationviewer.h"

#include "src/ctree.h"

class CPlayer : public QObject
{
    Q_OBJECT
private:
    explicit CPlayer(QObject *parent = 0);

    static CPlayer* s_pInstance;

    QGraphicsObject*     m_root_object;
    QDeclarativeContext* m_context;

    QSettings   m_settings;
    CTree       m_tree;

#if defined(MEEGO_EDITION_HARMATTAN)
    MeeGo::QmKeys* m_hwkeys;
#endif

    QStringList m_music_filters;
    QStringList m_cover_filters;

    QMediaPlayer*        m_player;

    QHash<QtMultimediaKit::MetaData, QString> m_metadata_list;
    QList<QtMultimediaKit::MetaData> m_metadata_list_order;

public:
    inline static CPlayer* getInstance() { if( s_pInstance == NULL ) s_pInstance = new CPlayer(); return s_pInstance; }
    inline static void     destroyInstance() { delete s_pInstance; }

    void initContext(QmlApplicationViewer& viewer);
    void initRoot(QmlApplicationViewer& viewer);

    Q_INVOKABLE inline QSettings* settings() { return &m_settings; }
    Q_INVOKABLE QVariant setting(QString key, QString value = "");

    Q_INVOKABLE inline CTree* tree() { return &m_tree; }
    Q_INVOKABLE inline QStringList* musicFilters() { return &m_music_filters; }
    Q_INVOKABLE inline QStringList* coverFilters() { return &m_cover_filters; }

    Q_INVOKABLE inline QString currentFile() { return CPlayer::getInstance()->setting("rplay/file").toString(); }
    Q_INVOKABLE inline QStringList currentFileArray() { return CPlayer::getInstance()->setting("rplay/file").toString().split('/'); }
    Q_INVOKABLE inline qint64 currentFilePosition() { return m_player->position(); }
    Q_INVOKABLE inline qint64 currentFileDuration() { return m_player->duration(); }

    Q_INVOKABLE ListModel* getMetaData();
    Q_INVOKABLE ListModel* getExtendedMetaData();
    Q_INVOKABLE ListModel* getLyrics(QString path);

    Q_INVOKABLE ListModel* prefsContent();

    Q_INVOKABLE void playFile(QString path);
    Q_INVOKABLE void playNext();
    Q_INVOKABLE void playRev();

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    
signals:
    void metaDataChanged();
    void next();
    
public slots:
    void statusChanged(QMediaPlayer::MediaStatus status);
#if defined(MEEGO_EDITION_HARMATTAN)
    void hwKeyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state);
#endif
    
};

#endif // CPLAYER_H

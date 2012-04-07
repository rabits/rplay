#ifndef CPLAYER_H
#define CPLAYER_H

#include <QObject>
#include <QSettings>
#include <QtDeclarative/QDeclarativeContext>
#include <QGraphicsObject>
#include <QtMultimediaKit/qmediaplayer.h>
#include <qmsystem2/qmkeys.h>
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

    MeeGo::QmKeys* m_hwkeys;

    QStringList m_music_filters;
    QStringList m_cover_filters;

    QMediaPlayer*        m_player;

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
    Q_INVOKABLE inline QString currentState() { return CPlayer::getInstance()->setting("rplay/state").toString(); }

    Q_INVOKABLE void playFile(QString path);
    Q_INVOKABLE void playNext();
    Q_INVOKABLE void playPrev();

    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    
signals:
    
public slots:
    void statusChanged(QMediaPlayer::MediaStatus status);
    void hwKeyEvent(MeeGo::QmKeys::Key key, MeeGo::QmKeys::State state);
    
};

#endif // CPLAYER_H

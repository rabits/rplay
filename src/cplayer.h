#ifndef CPLAYER_H
#define CPLAYER_H

#include <QObject>
#include <QSettings>
#include <QtMultimediaKit/qmediaplayer.h>

#include "src/ctree.h"

class CPlayer : public QObject
{
    Q_OBJECT
private:
    explicit CPlayer(QObject *parent = 0);

    static CPlayer* s_pInstance;

    QSettings   m_settings;
    CTree       m_tree;

    QStringList m_music_filters;
    QStringList m_cover_filters;

    QMediaPlayer *m_player;

    QString m_current_file;

public:
    inline static CPlayer* getInstance() { if( s_pInstance == NULL ) s_pInstance = new CPlayer(); return s_pInstance; }
    inline static void     destroyInstance() { delete s_pInstance; }

    Q_INVOKABLE inline QSettings* settings() { return &m_settings; }
    Q_INVOKABLE QVariant setting(QString key, QString value = "");

    Q_INVOKABLE inline CTree* tree() { return &m_tree; }
    Q_INVOKABLE inline QStringList* musicFilters() { return &m_music_filters; }
    Q_INVOKABLE inline QStringList* coverFilters() { return &m_cover_filters; }

    Q_INVOKABLE void playFile(QString path);
    Q_INVOKABLE void playNext();
    Q_INVOKABLE void playPrev();
    
signals:
    
public slots:
    void statusChanged(QMediaPlayer::MediaStatus status);
    
};

#endif // CPLAYER_H

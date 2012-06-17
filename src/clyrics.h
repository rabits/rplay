#ifndef CLYRICS_H
#define CLYRICS_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QFileInfo>

#include "listmodel.h"

class CLyrics
    : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool working READ working WRITE working NOTIFY workingChanged)

public:
    explicit CLyrics(QObject *parent = 0);

    inline QStringList& downloadedLyrics() { return m_downloaded_lyrics; }
    inline void downloadedLyricsAdd(QString &text) { m_downloaded_lyrics.append(text); emit lyricsChanged(); }
    inline void downloadedLyricsClear() { m_downloaded_lyrics.clear(); emit lyricsChanged(); }

    QString                 lyricName(QString path);
    Q_INVOKABLE void        lyricSave(QString text);
    Q_INVOKABLE inline bool lyricsHas(QString path) { return QFileInfo(lyricName(path)).isReadable(); }
    Q_INVOKABLE ListModel*  lyrics(QString path);

    Q_INVOKABLE void lyricsNet(QString artist, QString title);

    Q_INVOKABLE inline bool working() const           { return m_working; }
    Q_INVOKABLE inline void working(const bool value) { m_working = value; emit workingChanged(); }

signals:
    void workingChanged();
    void lyricsChanged();

private:
    QStringList m_downloaded_lyrics;

    QMutex m_mutex;
    volatile bool m_working;
    QString m_artist;
    QString m_title;

    QNetworkAccessManager *m_net_manager;

protected:
    void run();
};

#endif // CLYRICS_H

#ifndef CCOVER_H
#define CCOVER_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QNetworkAccessManager>
#include <QStringList>

class CCover
    : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool working READ working WRITE working NOTIFY workingChanged)

public:
    explicit CCover(QObject *parent = 0);

    Q_INVOKABLE QString cover(QString dir);
    Q_INVOKABLE bool coverHas(QString dir);
    Q_INVOKABLE void coverSave(QString dir, QByteArray data, QString suffix);

    Q_INVOKABLE void coverNet(QString artist, QString album, QString dir);

    Q_INVOKABLE inline bool working() const           { return m_working; }
    Q_INVOKABLE inline void working(const bool value) { m_working = value; emit workingChanged(); }

signals:
    void workingChanged();
    void coverChanged(QString filepath);

public slots:
    void coverNetCurrent();

private:
    QStringList m_cover_filters;

    QMutex m_mutex;
    volatile bool m_working;
    QString m_artist;
    QString m_album;

    QNetworkAccessManager *m_net_manager;
    QString                m_cover_dir;

protected:
    void run();
};

#endif // CCOVER_H

#ifndef CVOICE_H
#define CVOICE_H

#include <QThread>
#include <QMutex>

#include <flite.h>

extern "C" {
    cst_voice *register_cmu_us_slt();
}

class CVoice
    : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool working READ working WRITE working NOTIFY workingChanged)

public:
    explicit CVoice(QObject *parent = 0);

    Q_INVOKABLE inline bool working() const           { return m_working; }
    Q_INVOKABLE inline void working(const bool value) { m_working = value; emit workingChanged(); }

signals:
    void workingChanged();

public slots:
    void say(QString text);
    void sayCurrent();

private:
    QMutex m_mutex;
    volatile bool m_working;

    cst_voice* m_voice;
    QString    m_text;

    QString m_prev_artist_album;
    QString m_prev_title;

protected:
    void run();
};

#endif // CVOICE_H

#ifndef CVOICE_H
#define CVOICE_H

#include <QObject>
#include <QThread>

#include <flite.h>

extern "C" {
    cst_voice *register_cmu_us_slt();
}

class CVoiceThread : public QThread
{
    Q_OBJECT

public:
    explicit CVoiceThread(QObject *parent = 0);
    void setText(QString& text) { m_text = text; }
    void run();

private:
    cst_voice* m_voice;
    QString    m_text;
};

class CVoice : public QObject
{
    Q_OBJECT
public:
    explicit CVoice(QObject *parent = 0);

public slots:
    void say(QString text);
    void sayCurrent();

private:
    CVoiceThread m_thread;
    QString m_prev_artist_album;
    QString m_prev_title;
};

#endif // CVOICE_H

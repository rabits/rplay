#include "cvoice.h"
#include "src/cplayer.h"

CVoiceThread::CVoiceThread(QObject *parent) :
    QThread(parent)
{
    flite_init();
    m_voice = register_cmu_us_slt();
}

CVoice::CVoice(QObject *parent)
    : QObject(parent)
    , m_thread(this)
{
    qDebug("[rPlay] Init voice engine");
}

void CVoice::say(QString text)
{
    m_thread.setText(text);
    m_thread.start();
}

void CVoice::sayCurrent()
{
    if( CPlayer::getInstance()->setting("preferences/voice_say_on_meta_changed").toBool() != true )
        return;

    QString text;
    QString current;

    current = CPlayer::getInstance()->artist();
    current.append(". ").append(CPlayer::getInstance()->album());
    if( (CPlayer::getInstance()->setting("preferences/voice_artist_album_say").toBool() == true) &&
            current.compare(". ") != 0 && current.compare(m_prev_artist_album, Qt::CaseInsensitive) != 0 )
    {
        m_prev_artist_album = current;
        text.append(current).append(". ");
    }

    current = CPlayer::getInstance()->title();
    if( (CPlayer::getInstance()->setting("preferences/voice_title_say").toBool() == true) &&
            ! current.isEmpty() && current.compare(m_prev_title, Qt::CaseInsensitive) != 0 )
    {
        m_prev_title = current;
        text.append(current).append(". ");
    }

    if( ! text.isEmpty() )
        say(text);
}

void CVoiceThread::run()
{
    uint volume = CPlayer::getInstance()->volume();
    CPlayer::getInstance()->volume(50);
    flite_text_to_speech(m_text.toStdString().c_str(), m_voice, "play");
    CPlayer::getInstance()->volume(volume);
}

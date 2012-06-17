#include "cvoice.h"
#include "src/cplayer.h"

CVoice::CVoice(QObject *parent)
    : QThread(parent)
{
    qDebug("[rPlay] Init voice engine");

    flite_init();
    m_voice = register_cmu_us_slt();
}

void CVoice::say(QString text)
{
    if( ! m_mutex.tryLock() )
    {
        qDebug("[rPlay] Voice already working");
        return;
    }

    working(true);

    m_text = text;

    start();
}

void CVoice::sayCurrent()
{
    if( CPlayer::getInstance()->settingBool("preferences/voice_say_on_meta_changed") != true )
        return;

    QString text;
    QString current;

    current = CPlayer::getInstance()->artist();
    current.append(". ").append(CPlayer::getInstance()->album());
    if( (CPlayer::getInstance()->settingBool("preferences/voice_artist_album_say") == true) &&
            current.compare(". ") != 0 && current.compare(m_prev_artist_album, Qt::CaseInsensitive) != 0 )
    {
        m_prev_artist_album = current;
        text.append(current).append(". ");
    }

    current = CPlayer::getInstance()->title();
    if( (CPlayer::getInstance()->settingBool("preferences/voice_title_say") == true) &&
            ! current.isEmpty() && current.compare(m_prev_title, Qt::CaseInsensitive) != 0 )
    {
        m_prev_title = current;
        text.append(current).append(". ");
    }

    if( ! text.isEmpty() )
        say(text);
}

void CVoice::run()
{
    uint volume = CPlayer::getInstance()->volume();
    CPlayer::getInstance()->volume(50);
    flite_text_to_speech(m_text.toStdString().c_str(), m_voice, "play");
    CPlayer::getInstance()->volume(volume);

    working(false);
    m_mutex.unlock();
}

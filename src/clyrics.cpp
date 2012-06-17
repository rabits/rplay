#include "clyrics.h"

#include "cplayer.h"
#include "model/ckeyvalueitem.h"
#include <QNetworkReply>
#include <QEventLoop>
#include <QNetworkConfigurationManager>

CLyrics::CLyrics(QObject *parent)
    : QThread(parent)
    , m_downloaded_lyrics()
    , m_net_manager(NULL)
{
}

QString CLyrics::lyricName(QString path)
{
    if( path == "" )
        path = CPlayer::getInstance()->currentFile();
    path = CPlayer::getInstance()->tree()->libraryRoot(path);
    return path.left(path.length() - QFileInfo(path).suffix().length()).append(CPlayer::getInstance()->setting("preferences/lyrics_extension").toString());
}

void CLyrics::lyricSave(QString text)
{
    QFile lyrics(lyricName(""));
    if( ! lyrics.open(QFile::WriteOnly) )
        qDebug("[rPlay] Error: unable to write lyric into file");
    else
    {
        lyrics.write(text.toUtf8());
        lyrics.close();
        qDebug("[rPlay] Lyric saved");
    }
}

ListModel *CLyrics::lyrics(QString path)
{
    path = lyricName(path);
    ListModel *out = new ListModel(new CKeyValueItem(), parent());

    QFile lyrics(path);

    // If file less then 50Kb and readable
    if( (lyrics.size() < 50 * 1024) && lyrics.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        QTextStream in(&lyrics);
        out->appendRow(new CKeyValueItem(path, "Lyrics", in.readAll(), "lyric", this));
        lyrics.close();
    }

    if( ! downloadedLyrics().isEmpty() && CPlayer::getInstance()->settingBool("preferences/network_get_lyrics_show_always") )
    {
        foreach( QString lyric, downloadedLyrics() )
            out->appendRow(new CKeyValueItem(path, "Lyrics (downloaded)", lyric, "lyric", this));
    }

    if( out->rowCount() == 0 )
    {
        QUrl link("http://google.com/search");
        link.addQueryItem("q", CPlayer::getInstance()->artist() + " " + CPlayer::getInstance()->title() + " lyrics");
        out->appendRow(new CKeyValueItem(path, "Lyrics (<a href=\"" + QString(link.toEncoded()) + "\">try manual search</a>)", "", "lyric", this));
    }

    return out;
}

void CLyrics::lyricsNet(QString artist, QString title)
{
    if( !( CPlayer::getInstance()->settingBool("preferences/network_get_lyrics") && m_downloaded_lyrics.isEmpty() ) )
        return;

    // Get current access to network
    if( ! CPlayer::getInstance()->settingBool("preferences/network_autoconnect") )
    {
        QNetworkConfigurationManager manager;
        if( !manager.isOnline() )
            return;
    }

    if( ! m_mutex.tryLock() )
    {
        qDebug("[rPlay] Lyrics already requested");
        return;
    }

    working(true);

    m_artist = artist;
    m_title = title;

    start();
}

void CLyrics::run()
{
    QUrl url("http://webservices.lyrdb.com/lookup.php");
    QList< QPair<QString, QString> > query;
    query << QPair<QString, QString>("for", "match") << QPair<QString, QString>("q", m_artist + "|" + m_title)
          << QPair<QString, QString>("agent", "rPlay/"PROJECT_VERSION);
    url.setQueryItems(query);

    qDebug("[rPlay] Requesting lyrics");
    QEventLoop loop;
    QNetworkRequest request(url);
    m_net_manager = new QNetworkAccessManager();

    QNetworkReply *reply_info = m_net_manager->get(request);
    connect(reply_info, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    disconnect(&loop, SLOT(quit()));

    if( reply_info->error() )
        qDebug(QString("[rPlay] Lyrics info download error: ").append(reply_info->url().toString()).append(reply_info->errorString()).toStdString().c_str());
    else
    {
        qDebug("[rPlay] Lyrics info reply");
        QStringList info = QString(reply_info->readAll()).trimmed().split("\n");
        if( info.first() == "")
            qDebug("[rPlay] Lyrics not found for this track");
        else
        {
            foreach( QString id, info )
            {
                id = id.split("\\").first();
                if( ! id.isEmpty() )
                {
                    QUrl url("http://webservices.lyrdb.com/getlyr.php");
                    QList< QPair<QString, QString> > query;
                    query << QPair<QString, QString>("q", id) << QPair<QString, QString>("agent", "rPlay/"PROJECT_VERSION);
                    url.setQueryItems(query);

                    QNetworkRequest request(url);
                    QNetworkReply *reply_data = m_net_manager->get(request);

                    connect(reply_data, SIGNAL(finished()), &loop, SLOT(quit()));
                    loop.exec();
                    disconnect(&loop, SLOT(quit()));

                    if( reply_data->error() )
                        qDebug(QString("[rPlay] Lyrics data download error: ").append(reply_data->url().toString()).append(reply_data->errorString()).toStdString().c_str());
                    else
                    {
                        qDebug("[rPlay] Lyrics data reply");
                        QString data = QString(reply_data->readAll()).trimmed().replace('\r', "");
                        if( ! data.isEmpty() )
                        {
                            downloadedLyricsAdd(data);
                            if( CPlayer::getInstance()->settingBool("preferences/network_get_lyrics_autosave") && !lyricsHas("") )
                                lyricSave("[autosaved]\n\n" + data);
                        }
                        else
                            qDebug("[rPlay] Lyrics data not found for this track");
                    }

                    delete reply_data;
                }
            }
        }
    }

    delete reply_info;
    delete m_net_manager;

    working(false);
    m_mutex.unlock();
}

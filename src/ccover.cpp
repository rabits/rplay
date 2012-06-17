#include "ccover.h"

#include "cplayer.h"
#include <QNetworkReply>
#include <QEventLoop>
#include <QDir>
#include <QNetworkConfigurationManager>

CCover::CCover(QObject *parent)
    : QThread(parent)
    , m_net_manager(NULL)
{
    m_cover_filters << "cover.jpg" << "cover.jpeg" << "cover.png" << "folder.jpg" << "folder.jpeg" << "folder.png";
}

QString CCover::cover(QString dir)
{
    if( dir == "" )
        dir = CPlayer::getInstance()->tree()->parentDir(CPlayer::getInstance()->currentFile());

    QDir folder = QDir(CPlayer::getInstance()->tree()->libraryRoot(dir));
    QStringList cover_list = folder.entryList(m_cover_filters, QDir::Files);

    if( ! cover_list.isEmpty() )
        return folder.path() + "/" + cover_list.first();

    return "";
}

bool CCover::coverHas(QString dir)
{
    if( dir == "" )
        dir = CPlayer::getInstance()->tree()->parentDir(CPlayer::getInstance()->currentFile());

    QDir folder = QDir(CPlayer::getInstance()->tree()->libraryRoot(dir));
    QStringList cover_list = folder.entryList(m_cover_filters, QDir::Files);

    return ! cover_list.isEmpty();
}

void CCover::coverNetCurrent()
{
    coverNet(CPlayer::getInstance()->artist(), CPlayer::getInstance()->album(), "");
}

void CCover::coverNet(QString artist, QString album, QString dir)
{
    if( dir == "" )
        dir = CPlayer::getInstance()->tree()->parentDir(CPlayer::getInstance()->currentFile());

    if( (! CPlayer::getInstance()->settingBool("preferences/network_get_covers"))
            || (m_artist == artist && m_album == album) || coverHas(dir) )
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
        qDebug("[rPlay] Cover already requested");
        return;
    }

    working(true);

    m_artist = artist;
    m_album = album;
    m_cover_dir = dir;

    start();
}

void CCover::run()
{
    // Gets cover from lastfm
    QUrl info_url("http://ws.audioscrobbler.com/2.0/");
    QList< QPair<QString, QString> > query;
    query << QPair<QString, QString>("method", "album.getinfo") << QPair<QString, QString>("api_key", "b25b959554ed76058ac220b7b2e0a026")
          << QPair<QString, QString>("artist", m_artist) << QPair<QString, QString>("album", m_album);
    info_url.setQueryItems(query);

    qDebug("[rPlay] Requesting cover");
    QNetworkRequest request(info_url);
    m_net_manager = new QNetworkAccessManager();
    QNetworkReply *reply_info = m_net_manager->get(request);

    QEventLoop loop;
    connect(reply_info, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    disconnect(&loop, SLOT(quit()));

    if( reply_info->error() )
        qDebug(QString("[rPlay] Cover info download error: ").append(reply_info->url().toString()).append(reply_info->errorString()).toStdString().c_str());
    else
    {
        qDebug("[rPlay] Cover info reply");
        QStringList info = QString(reply_info->readAll()).replace('>', '\n').replace('<', '\n').split("\n");
        QUrl data_url;
        for( int i = 0; i < info.count(); i++ )
        {
            if( info.at(i) == "image size=\"extralarge\"" )
            {
                data_url = QUrl(info.at(i+1));
                break;
            }
        }

        if( data_url.isEmpty() )
            qDebug("[rPlay] Cover not found for this album");
        else
        {
            QNetworkRequest request(data_url);
            QNetworkReply *reply_data = m_net_manager->get(request);

            connect(reply_data, SIGNAL(finished()), &loop, SLOT(quit()));
            loop.exec();
            disconnect(&loop, SLOT(quit()));

            if( reply_data->error() )
                qDebug(QString("[rPlay] Cover data download error: ").append(reply_data->url().toString()).append(reply_data->errorString()).toStdString().c_str());
            else
            {
                qDebug("[rPlay] Cover data reply");
                QByteArray data = reply_data->readAll();
                if( ! data.isEmpty() )
                    coverSave(m_cover_dir, data, QFileInfo(reply_data->url().path()).suffix());
                else
                    qDebug("[rPlay] Cover data not found for this track");
            }

            delete reply_data;
        }
    }

    delete reply_info;
    delete m_net_manager;

    working(false);
    m_mutex.unlock();
}

void CCover::coverSave(QString dir, QByteArray data, QString suffix)
{
    QFile file(CPlayer::getInstance()->tree()->libraryRoot(dir + "/cover." + suffix));
    if( !file.open(QIODevice::WriteOnly) )
         qDebug(QString("[rPlay] Could not open file for writing: ").append(file.fileName()).toStdString().c_str());
    else
    {
        file.write(data);
        file.close();
        emit coverChanged(file.fileName());
    }
}

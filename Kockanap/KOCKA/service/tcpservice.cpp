#include "tcpservice.h"

#include <QDebug>

#include <QString>
#include <ios>
#include <QStringList>
#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

TcpService::TcpService(QObject *parent) : QObject(parent)
{

}

void TcpService::sendSS()
{

    //ss.seekp(0,std::ios::end);

    //for(int i=0; i< ss.tellp();++i)
        //std::cout << ss.str()[i];
    //_pSocket->waitForBytesWritten(ss.tellp());
    //int a= _pSocket->write(ss.str().c_str(),ss.tellp());
    //_pSocket->flush();

    //qDebug() << "ennyit: " << a;

    //ss.str(std::string());
}

void TcpService::writeData(std::string msg) {
    qDebug() << QString::fromStdString(msg);

    QByteArray sizeByte;
    int size = msg.size();
    sizeByte.append(reinterpret_cast<const char*>(&size),4);
    sizeByte.append(msg.c_str());

    while( _pSocket->write(sizeByte) != size ){
        _pSocket->waitForBytesWritten();
    };
    std::cout << "Sended Datas:" << size << std::endl;
   _pSocket->flush();
    //ss << std::string(reinterpret_cast<const char*>(&size),4) << msg;

}

TcpService::~TcpService() {

    _pSocket->close();
}

void TcpService::readTcpData()
{

    int x;
    _pSocket->read(reinterpret_cast<char*>(&x),4);

    std::string message;
    char asd[512];
    int t = 0;
    while( (t += _pSocket->read( asd , std::min( 511 , x-t ) )) != x )
    {

        qDebug() << asd;
        message += asd;
    }

    qDebug() << x ;
    //qDebug() << message;

    if(message.find("GAMEITEMS") != -1) {

        //QStringList arr = message.split("]");
        auto it = std::find( message.begin() , message.end() , ']' );
        auto it2 = std::find( it , message.end() , ']' );

        sendData( std::string( it+1 , it2 ));
    } else if( message.find("EVENT:RESET") != -1) {

        _pSocket->disconnectFromHost();

        _pSocket->connectToHost("192.168.1.99",6666);
        connect( _pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );


        qDebug() << "reconnected";

    } else if( message.find("MAP") != -1) {
        //qDebug() << message;
        auto it = std::find( message.begin() , message.end() , ':' );
        auto it2 = std::find( it , message.end() , ':' );

        setMap(std::string(it, it2));
    }
    _pSocket->flush();
}

//nem kell
void TcpService::getTime(int _time) {
    //qDebug() << time;
    if(time == 0) {
        qDebug() << "over" << endl;
        reset();
    }
}

void TcpService::connectTcp()
{
    _pSocket = new QTcpSocket( this );
    connected = false;
    _pSocket->abort();

    connect( _pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );
    connect(_pSocket, SIGNAL(error(QAbstractSocket::SocketError)),
   this, SLOT(error(QAbstractSocket::SocketError)));

    _pSocket->connectToHost("192.168.1.99",6666);
    if( _pSocket->waitForConnected() ) {
        connected = true;
        qDebug() << "Csatlakozott";
    }
}

void TcpService::error(QAbstractSocket::SocketError err){
    qDebug() << "err" << err;

}

void TcpService::login(std::string username , std::string password) {

  writeData("/LOGIN "+username+" "+password);

}

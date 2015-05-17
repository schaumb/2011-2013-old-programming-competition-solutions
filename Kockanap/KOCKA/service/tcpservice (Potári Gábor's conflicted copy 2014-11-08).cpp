#include "tcpservice.h"

#include <QDebug>
#include <QApplication>
#include <QString>
#include <ios>
#include <QStringList>

TcpService::TcpService(QObject *parent) : QObject(parent)
{

}

void TcpService::sendSS()
{

    ss.seekp(0,std::ios::end);

    //for(int i=0; i< ss.tellp();++i)
        //std::cout << ss.str()[i];
    _pSocket->waitForBytesWritten(ss.tellp());
    int a= _pSocket->write(ss.str().c_str(),ss.tellp());
    _pSocket->flush();
    qDebug() << "ennyit: " << a;

    ss.str(std::string());
}

void TcpService::writeData(std::string msg) {
    qDebug() << QString::fromStdString(msg);

        //_pSocket->write(msg.c_str());

    QByteArray sizeByte;
    int size = msg.size();
    sizeByte.append(reinterpret_cast<const char*>(&size),4);
    sizeByte.append(msg.c_str());

    //_pSocket->write(sizeByte);
    ss << std::string(reinterpret_cast<const char*>(&size),4) << msg;

}

void TcpService::readTcpData()
{
    QByteArray data = _pSocket->readAll();
    //qDebug() << data;
    QString message(data);

    data = data.remove(0,3);

    if(message.contains("GAMEITEMS")) {
        QStringList arr = message.split("]");

        sendData( (arr[1].remove(0,1)).toStdString());
    } else if( message.contains("EVENT:RESET")) {

        _pSocket->disconnectFromHost();

        _pSocket->connectToHost("192.168.1.99",6666);
        connect( _pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );


        qDebug() << "reconnected";

    } else if( message.contains("MAP") ) {
        qDebug() << message;
        setMap(message.split(":")[1].toStdString());
    }
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
    qDebug() << "err";
}

void TcpService::login(std::string username , std::string password) {

  writeData("/LOGIN "+username+" "+password);

}

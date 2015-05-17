#include "tcpservice.h"

#include <QDebug>
#include <QApplication>
#include <QString>
#include <ios>
#include <QStringList>

static inline qint32 ArrayToInt(QByteArray source);

TcpService::TcpService(QObject *parent) : QObject(parent)
{
    _pSocket = new QTcpSocket(this);
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
        _pSocket->waitForBytesWritten(1000);
    };
    std::cout << "Sended Datas:" << size << std::endl;
   //_pSocket->flush();
    //ss << std::string(reinterpret_cast<const char*>(&size),4) << msg;

}

TcpService::~TcpService() {

    _pSocket->close();
}


qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

void TcpService::readTcpData()
{

    QByteArray inputData;



       qDebug() << "maradt bájtok: " << _pSocket->bytesAvailable();
       QByteArray thisBuffer;
       thisBuffer = _pSocket->readAll();//_pSocket->read(_pSocket->bytesAvailable());


            inputData += thisBuffer;

       QString message = QString(inputData).trimmed();

        qDebug() << message;
       if(message.contains("GAMEITEMS")) {
      //  qDebug() << message;
           QStringList arr = message.split("]");

           sendData( arr[1].toStdString());
       } else if( message.contains("EVENT:RESET")) {

        //   _pSocket->disconnectFromHost();

          // _pSocket->connectToHost("192.168.1.99",6666);
       //    connect( _pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );

       } else if( message.contains("MAP")) {
           qDebug() << message.trimmed();
           setMap(message.split(":")[1].trimmed().toStdString());
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
//    _pSocket = new QTcpSocket( this );



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

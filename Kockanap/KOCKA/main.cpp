#include "main.h"
#include <iostream>
#include <QDebug>



Main::Main(QObject *parent):logic(this,"doubleInf")
{

}

void Main::writeData(std::string msg) {
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


std::map< UL , std::string > num_to_what{ { 0 , "SpaceBattleCommon.Planet" } , { 1 , "SpaceBattleCommon.Ship" } };
std::map< std::string , UL > what_to_num{ { "SpaceBattleCommon.Planet" , 0 } , { "SpaceBattleCommon.Ship" , 1 } };




void Main::writeResult(std::string result) {
    //ui->info->setText(QString::fromStdString(result));
}

void Main::writeMessage(std::string param) {
    writeData(param);
}

void Main::on_pushButton_clicked()
{
    QTcpSocket
    connectTcp();

    //connect(service,SIGNAL(sendData(std::string)),this,SLOT(getMessageFromTCP(std::string)));
    //connect(service,SIGNAL(reset()),this,SLOT(reset()));
    //connect(service,SIGNAL(setMap(std::string)),this,SLOT(setMap(std::string)));
    //login("doubleInf","doublePass");

}

void Main::setMap(std::string map) {
    double x =  QString::fromStdString(map).split("x")[0].toDouble();
    double y =  QString::fromStdString(map).split("x")[1].toDouble();
    qDebug() << x << y ;
    logic.setMap(x,y);

}

void Main::reset() {
    logic.reset();
}

void Main::getMessageFromTCP(std::string msg){

    logic.getData(msg);
}

std::string createShip(long bolygo, int c) {
    return("/SPLIT "+std::to_string(bolygo)+" "+std::to_string(c)+"\n");

}

std::string moveShip(long id,int x ,int y) {

    return("/MOVE "+std::to_string(id)+" "+std::to_string(x)+" "+std::to_string(y)+"\n");
}

std::string Shoot(long ship , long bolygo,int units) {
    return("/SHOOT "+std::to_string(ship)+" "+std::to_string(bolygo)+" "+std::to_string(units)+"\n");
}


void Main::readTcpData()
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

        //sendData( std::string( it+1 , it2 ));
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
void Main::setTime(int _time) {
    //qDebug() << time;
    if(time == 0) {
        qDebug() << "over" << endl;
        reset();
    }
}

void Main::connectTcp()
{
    _pSocket = new QTcpSocket( this );
    _pSocket->abort();

    connect( _pSocket, SIGNAL(readyRead()), this, SLOT(readTcpData()) );
    connect(_pSocket, SIGNAL(error(QAbstractSocket::SocketError)),
   this, SLOT(error(QAbstractSocket::SocketError)));

    _pSocket->connectToHost("192.168.1.99",6666);
    if( _pSocket->waitForConnected() ) {

        qDebug() << "Csatlakozott";
    }
}

void TcpService::error(QAbstractSocket::SocketError err){
    qDebug() << "err" << err;

}

void TcpService::login(std::string username , std::string password) {

  //writeData("/LOGIN "+username+" "+password);

}

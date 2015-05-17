#ifndef TCPSERVICE_H
#define TCPSERVICE_H

#include <QByteArray>
#include <iostream>
#include <QTcpSocket>
#include <QObject>
#include <sstream>
#include <QHash>
#include <QTcpServer>

class TcpService : public QObject
{
    Q_OBJECT

public:
    TcpService(QObject *parent = 0);
    ~TcpService();
    void writeData(std::string msg);
    void connectTcp();
    void sendSS();


    //tcp üzenetek
    void login(std::string username , std::string password);

private slots:
    void readTcpData();
    void error(QAbstractSocket::SocketError err);


signals:
    void sendData(std::string msg);
    void reset();
    void setMap(std::string map);
     void dataReceived(QByteArray);


private:
    QTcpServer server;
    void getTime(int _time);
    QByteArray data;
    bool connected;
    QTcpSocket* _pSocket;
    std::stringstream ss;
    int time;
    QHash<QTcpSocket*, QByteArray*> buffers;
    QHash<QTcpSocket*, qint32*> sizes;
     int bullshit;

};

#endif // TCPSERVICE_H

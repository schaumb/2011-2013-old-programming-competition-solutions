#ifndef MAIN_H
#define MAIN_H



#include <string>

std::string createShip(long bolygo, int c);
std::string moveShip(long id, int x , int y);
std::string Shoot(long ship , long bolygo, int units);


#include <logic/gamelogic.h>
#include <service/tcpservice.h>
#include <QObject>

class Main: public QObject
{
    Q_OBJECT

public:
    explicit Main(QObject *parent = 0);
    void writeResult(std::string result);
    void connectTcp();
    void writeMessage(std::string param);
    void writeData(std::string msg);
    void readTcpData();

public slots:
    void on_pushButton_clicked();

private:

    void getMessageFromTCP(std::string msg);
    void reset();
    void setMap(std::string map);
    void setTime(int _time);
private:
    gamelogic<Main> logic;
    QTcpSocket* _pSocket;

};

#endif // MAIN

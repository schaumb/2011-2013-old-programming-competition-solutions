#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <string>

std::string createShip(long bolygo, int c);
std::string moveShip(long id, int x , int y);
std::string Shoot(long ship , long bolygo, int units);

#include <QMainWindow>
#include "service/tcpservice.h"
#include <logic/gamelogic.h>
#include <QApplication>

namespace Ui {
    class MainWindow;
}


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    void writeResult(std::string result);
    void connectTcp();
    void writeMessage(std::string param);
public slots:
    void on_pushButton_clicked();
private slots:
    void getMessageFromTCP(std::string msg);
    void reset();
    void setMap(std::string map);

private:
    Ui::MainWindow *ui;
    TcpService *service;
    gamelogic<MainWindow> logic;

};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>

std::map< UL , std::string > num_to_what{ { 0 , "SpaceBattleCommon.Planet" } , { 1 , "SpaceBattleCommon.Ship" } };
std::map< std::string , UL > what_to_num{ { "SpaceBattleCommon.Planet" , 0 } , { "SpaceBattleCommon.Ship" , 1 } };


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    logic(this,"doubleInf")
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::writeResult(std::string result) {
    //ui->info->setText(QString::fromStdString(result));
}

void MainWindow::writeMessage(std::string param) {
    service->writeData(param);
}

void MainWindow::on_pushButton_clicked()
{
    service = new TcpService();

    //service->readTcpData();
    connect(service,SIGNAL(sendData(std::string)),this,SLOT(getMessageFromTCP(std::string)));
    connect(service,SIGNAL(reset()),this,SLOT(reset()));
    connect(service,SIGNAL(setMap(std::string)),this,SLOT(setMap(std::string)));
    service->connectTcp();
    service->login("doubleInf","doublePass");
    //service->sendSS();
}

void MainWindow::setMap(std::string map) {
    double x =  QString::fromStdString(map).split("x")[0].toDouble();
    double y =  QString::fromStdString(map).split("x")[1].toDouble();
    qDebug() << x << y ;
    logic.setMap(x,y);

}

void MainWindow::reset() {
    logic.reset();
}

void MainWindow::getMessageFromTCP(std::string msg){

    logic.getData(msg);
   // service->sendSS();
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



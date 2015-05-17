#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include "tokenize.h"
#include <unordered_map>
#include <QThread>

template <typename T >
class gamelogic
{
    T* parent;
    std::string PlayerName;
    std::unordered_map < UL , moveable_item<> > hajo_mi;
    std::unordered_map < UL , item<> > hajo_mas;

    std::unordered_map < UL , item<> > bolygok;
    std::unordered_map < UL , item<> > bolygo_mi;
    std::unordered_map < UL , item<> > bolygo_mas;

    bool setted;

    enum all{ FELFEDEZO , TAMADAS , VEDEKEZES_LOKALIS , VEDEKEZES_GLOBAL , VEGALLAPOT  } allapot;
    double x,y;
public:
    gamelogic( T* p , std::string name ):parent( p ),PlayerName(name){
        setted = false;
    }

    void getData( std::string message )
    {
        hajo_mas.clear();
        bolygo_mas.clear();
        bolygo_mi.clear();

        auto items = tokenize( message);

        for( auto &x : items )
        {
            switch( x.What )
            {
                case 0:
                {
                    bolygok[x.ItemId] = x;
                    if( PlayerName == x.ItemPlayer )
                    {
                        bolygo_mi[x.ItemId] = x;
                    }
                    else
                    {
                        bolygo_mas[x.ItemId] = x;
                    }
                }
                break;
                case 1:
                {
                    if( PlayerName == x.ItemPlayer )
                    {
                        hajo_mi[x.ItemId] = x;
                    }
                    else
                    {
                        hajo_mas[x.ItemId] = x;
                    }
                }
                break;
            }
        }
        doLogic();
        setted = true;
    }
    void reset()
    {
        bolygok.clear();
    }

    void setMap( double sx , double sy )
    {
        x = sx;
        y = sy;

    }


private:
    void doLogic()
    {
        if(!setted) {
            moveShips();
        }

        for(auto ship: hajo_mi) {
           // qDebug() << ship.second.PosY;
            ship.second.move();
           // qDebug() << ship.second.PosY;

            parent->moveShip(ship.first,ship.second.PosX,ship.second.PosY);
        }

        for(auto bolygo: bolygo_mi) {
            //qDebug() << bolygo.second.Units;
            if(bolygo.second.Units > 0)
                createShip(bolygo.first,1);
            parent->writeResult(std::to_string(bolygo.second.PosX)+" "+std::to_string(bolygo.second.PosY));
            parent->writeResult("ships: " + std::to_string(hajo_mi.size()));
        }

        switch( allapot )
        {
        case FELFEDEZO : felfedezes(); break;
        case TAMADAS : tamadas(); break;
        case VEDEKEZES_LOKALIS : lokalved(); break;
        case VEDEKEZES_GLOBAL : globalved(); break;
        case VEGALLAPOT : vegall(); break;
        }
    }

    void moveShips() {


        for(auto ship: hajo_mi) {
            ship.second.setDirection(0.0,0.0);
        }
    }

    void createShip(long bolygo,  int c) {
        parent->createShip(bolygo,c);
    }

    void felfedezes()
    {
    }

    void tamadas()
    {

    }

    void lokalved()
    {
    }

    void globalved()
    {
    }

    void vegall()
    {
    }
};

#endif // GAMELOGIC_H

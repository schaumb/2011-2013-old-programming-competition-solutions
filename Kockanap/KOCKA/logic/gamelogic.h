#ifndef GAMELOGIC_H
#define GAMELOGIC_H
#include "tokenize.h"
#include <unordered_map>
#include <QThread>
#include <set>
#include <map>

#define KEZDETI_FELES 2
#define MEGEROSODES 6

template <typename T >
class gamelogic
{
    T* parent;
    std::string PlayerName;
    std::unordered_map < UL , moveable_item<> > hajo_mi;
    std::unordered_map < UL , item<> > hajo_mas;

    std::map < UL , item<> > bolygok;
    std::map < UL , item<> > bolygo_mi;
    std::unordered_map < UL , item<> > bolygo_mas;
    std::unordered_map < UL , item<> > bolygo_senki;


    enum all{ FELFEDEZO , TAMADAS_NEM_ELLENFEL , TAMADAS_ELLENFEL , VEDEKEZES_LOKALIS , VEDEKEZES_GLOBAL , VEGALLAPOT  } allapot;
    double x,y;
public:
    gamelogic( T* p , std::string name ):parent( p ),PlayerName(name){
        allapot = FELFEDEZO;
    }

    void getData( std::string message )
    {

        hajo_mas.clear();
        bolygo_mas.clear();
        bolygo_mi.clear();
        bolygo_senki.clear();

        std::set< UL > nem_halott;
        auto items = tokenize( message,x,y);

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
                    else if ( x.ItemPlayer.empty() )
                    {
                        bolygo_senki[x.ItemId] = x;
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
                        nem_halott.insert( x.ItemId );
                        // TODO frissiteni a halottakat!!!!
                        auto it = hajo_mi.find( x.ItemId );
                        if( it == std::end( hajo_mi ) )
                        {
                            hajo_mi[x.ItemId] = x;
                        }
                        else
                        {   

                            hajo_mi[x.ItemId].PosX = x.PosX;
                            hajo_mi[x.ItemId].PosY = x.PosY;
                            hajo_mi[x.ItemId].Units = x.Units;
                        }
                    }
                    else
                    {
                        hajo_mas[x.ItemId] = x;
                    }
                }
                break;
            }
        }

        for( auto it = hajo_mi.begin() ; it != hajo_mi.end() ; )
        {
            if( !nem_halott.count( it->first ) )
            {
                it = hajo_mi.erase( it );

            } else {
                ++it;
            }
        }

        doLogic();

    }
    void reset()
    {
        bolygok.clear();
    }

    void setMap( double sx , double sy )
    {

        x = sx;
        y = sy;
        allapot = FELFEDEZO;

    }


private:

    void doLogic()
    {
        switch( allapot )
        {
        case FELFEDEZO : felfedezes(); break;
        case TAMADAS_NEM_ELLENFEL : tamadas_nem(); break;
        case TAMADAS_ELLENFEL : tamadas(); break;
        case VEDEKEZES_LOKALIS : lokalved(); break;
        case VEDEKEZES_GLOBAL : globalved(); break;
        case VEGALLAPOT : vegall(); break;
        }
    }

    void felfedezes()
    {
        std::set<UL> uresBolygok;

        for(auto it : bolygok) {
            if(it.second.ItemPlayer.empty() ) {
                uresBolygok.insert(it.first);
            }
        }

        int felfedezok = 0;
        int masok = 0;
        
        int ures_bolyg = 0;
       // qDebug() << hajo_mi.size();
        for(auto ship: hajo_mi) 
        {
            if( ship.second.allapot == moveable_item<>::NINCS )
            {
                //irány az új bolygó
                if( felfedezok > 6 || ship.second.Units > 1 ||  uresBolygok.size()  )
                {

                       // auto& bolygo = bolygok[*std::next( std::begin( uresBolygok ) , ures_bolyg++ % uresBolygok.size() )];
                         auto& bolygo = bolygok[*std::begin( uresBolygok )];
                         long l =  *uresBolygok.begin();
                         //qDebug() << "itány az új bolygó- ki: "<< ship.first<<" x: " << bolygo.PosX << " y " << bolygo.PosY;

                        ship.second.setDirection( bolygo.PosX , bolygo.PosY ,
                                                  moveable_item<>::TAMAD , moveable_item<>::BALFEL , &bolygo );
                    /*
                        ship.second.setDirection( std::begin(bolygo_mi)->second.PosX , 
                                                std::begin(bolygo_mi)->second.PosY , 
                                            moveable_item<>::VEDEKEZ_MAS );
                    */
                }
                else
                {
                    //keresés
                    ship.second.setDirection( std::begin(bolygo_mi)->second.PosX , 
                                                std::begin(bolygo_mi)->second.PosY , 
                                            moveable_item<>::FELFEDEZ,
                                            static_cast<moveable_item<>::ira >(felfedezok % 4) , &std::begin(bolygo_mi)->second );
                }
            }
            
            if( ship.second.allapot == moveable_item<>::FELFEDEZ )
            {
                ++felfedezok;
                auto messages = ship.second.move();
                for( auto message : messages )
                {
                    parent->writeMessage( message );
                }
                //parent->moveShip(ship.first,ship.second.currentX,ship.second.currentY);

            }
            else
            {
                ++masok;
                auto messages = ship.second.move();

                for( auto message : messages )
                {
                    parent->writeMessage( message );
                }
            }
        }


        for(auto bolygo: bolygo_mi) {


            std::set<UL>::iterator i = uresBolygok.begin();

            if ( uresBolygok.size() && felfedezok > 2 && bolygo.second.Units > bolygok[*i].Units + KEZDETI_FELES )
            {
                //qDebug() << "Nagy hajó készítése foglaláshoz méret:" <<  bolygok[*i].Units + 1;
                parent->writeMessage( createShip( bolygo.first,1 ) );
            }
            else if(bolygo.second.Units > 0 && felfedezok < 7)
            {
                for( int i = 6 - felfedezok + KEZDETI_FELES ; i < bolygo.second.Units ; ++i )
                   parent->writeMessage( createShip(bolygo.first,1) );
            }
            else if( bolygo.second.Units > MEGEROSODES )
            {
                parent->writeMessage( createShip( bolygo.first, bolygo.second.Units/2 ) );
               // allapot = TAMADAS_NEM_ELLENFEL;
            }
                // gyujtogetes


            parent->writeResult(std::to_string(bolygo.second.PosX)+" "+std::to_string(bolygo.second.PosY));
            parent->writeResult("ships: " + std::to_string(hajo_mi.size()));

            
        }



    }

    void tamadas_nem()
    {

        /*
        std::set<UL> uresBolygok;

        for(auto it : bolygok) {
            if(it.second.ItemPlayer.empty() ) {
                uresBolygok.insert(it.first);
            }
        }

        if(uresBolygok.size() > 0) {
            for(auto ship: hajo_mi)
            {
                auto& bolygo = bolygok[* std::begin( uresBolygok )];

                ship.second.setDirection( bolygo.PosX , bolygo.PosY , moveable_item<>::TAMAD , moveable_item<>::BALFEL , &bolygo );

                auto messages = ship.second.move();
                for( auto message : messages )
                {
                    parent->writeMessage( message );
                }
            }
        } else {
            allapot = TAMADAS_ELLENFEL;
        }
        */
    }

    void tamadas()
    {

        std::set<UL> targetBolygok;

        for(auto it : bolygok) {
            if( it.second.ItemPlayer != PlayerName  ) {
                targetBolygok.insert(it.first);
            }
        }

        if(targetBolygok.size() > 0) {
            for(auto ship: hajo_mi)
            {
                auto& bolygo = bolygok[* std::begin( targetBolygok )];
                ship.second.setDirection( bolygo.PosX , bolygo.PosY , moveable_item<>::TAMAD , moveable_item<>::BALFEL , &bolygo );

                auto messages = ship.second.move();
                for( auto message : messages )
                {
                    parent->writeMessage( message );
                }
            }
        }


        for(auto bolygo: bolygo_mi) {


            std::set<UL>::iterator i = targetBolygok.begin();

            if ( bolygo.second.Units > bolygok[*i].Units + 1 )
            {
                //qDebug() << "elmentem foglalni, mert felfedeztem egy bolygót" ;
                parent->writeMessage( createShip( bolygo.first, bolygok[*i].Units + 1 ) );

            }
            else if( bolygo.second.Units > 0 )
            {
                parent->writeMessage( createShip( bolygo.first, bolygo.second.Units / 2 ) );
            }

            ++i;
            if( i == targetBolygok.end() ) i = targetBolygok.begin();
            parent->writeResult(std::to_string(bolygo.second.PosX)+" "+std::to_string(bolygo.second.PosY));
            parent->writeResult("ships: " + std::to_string(hajo_mi.size()));


        }

    }

    void lokalved()
    {

    }

    void globalved()
    {
        //túlélés



        for(auto ship: hajo_mi)
        {
            auto& bolygo =  bolygo_mi[0];
            ship.second.setDirection( bolygo.PosX , bolygo.PosY , moveable_item<>::TAMAD , moveable_item<>::BALFEL , &bolygo );

            auto messages = ship.second.move();
            for( auto message : messages )
            {
                parent->writeMessage( message );
            }
        }

    }

    void vegall()
    {
    }
};

#endif // GAMELOGIC_H

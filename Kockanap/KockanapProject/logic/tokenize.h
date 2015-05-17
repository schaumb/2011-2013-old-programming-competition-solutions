#ifndef TOKENIZE_H
#define TOKENIZE_H
#include <istream>
#include <sstream>
#include <map>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>

#define LEPES 10

typedef unsigned long UL;

extern std::map< UL , std::string > num_to_what;
extern std::map< std::string , UL > what_to_num;


template <typename T = UL>
struct item
{
    UL What,ItemId;
    std::string ItemPlayer;
    double PosX,PosY;
    UL Units,IncTime;

    virtual ~item(){}


    item( std::string buff = "" )
    {
        std::stringstream ss( buff );
        while( std::getline( ss , buff , ';' ) )
        {
            auto it = std::find( buff.begin() , buff.end() , '=' );
            beallit( buff.begin() , it , buff.end() );
        }
    }

    template <typename U>
    void beallit( U begin , U middle , U end )
    {
        if( std::equal( begin , middle , "What      " ) )
        {
            What = what_to_num[ std::string( middle + 1 , end ) ];
        }
        else if( std::equal( begin , middle , "ItemId    " ) )
        {
            ItemId = std::stoul( std::string( middle + 1 , end ) );
        }
        else if( std::equal( begin , middle , "ItemPlayer" ) )
        {
            auto nevend = std::find( middle + 1 , end , ' ' );
            ItemPlayer.assign( middle + 1 , nevend );
        }
        else if( std::equal( begin , middle , "PosX      " ) )
        {
            PosX = std::stod( std::string( middle + 1 , end ) );
        }
        else if( std::equal( begin , middle , "PosY      " ) )
        {
            PosY = std::stod( std::string( middle + 1 , end ) );
        }
        else if( std::equal( begin , middle , "Units     " ) )
        {
            Units = std::stoul( std::string( middle + 1 , end ) );
        }
        else if( std::equal( begin , middle , "IncTime   " ) )
        {
            IncTime = std::stoul( std::string( middle + 1 , end ) );
        }
    }


};

template <typename T = UL>
struct moveable_item: item<T>
{
    enum all{ NINCS , FELFEDEZ , TAMAD , VEDEKEZ_MAGA , VEDEKEZ_MAS } allapot;
    enum ira{ BALFEL = 0 , JOBBFEL , JOBBLE , BALLE } irany,most,kovi;
    double currentX;
    double currentY;
    item<>* targetp;
    double sideLength;
    UL max_palya_x;
    UL max_palya_y;
    
    
   
    moveable_item( std::string buff = "" , UL x= 0 , UL y= 0 ): item<T>(buff),allapot(NINCS),max_palya_x(x),max_palya_y(y){}

    bool areWeThere() {
        if(this->PosX == currentX && this->PosY == currentY) {
            return true;
        }
        return false;
    }

    void setDirection(double x, double y , all allap , ira iranyx = BALFEL, item<>* target = NULL) 
    {
        targetp = target;
        currentX = x;
        currentY = y;
        allapot = allap;

        irany = iranyx;
        switch( irany )
        {
            case BALFEL : currentX -= LEPES; currentY -= LEPES; break;
            case JOBBFEL : currentX += LEPES; currentY -= LEPES; break;
            case BALLE : currentX -= LEPES; currentY += LEPES; break;
            case JOBBLE : currentX += LEPES; currentY += LEPES; break;
        }
        most = irany;
        kovi = static_cast<ira>(static_cast<int>(irany)+1%4);
        sideLength = LEPES;

    }

    std::vector<std::string> move() 
    {
       // qDebug() << allapot;
        std::vector<std::string> res;
        switch( allapot )
        {
            case FELFEDEZ :
            {

                if( std::abs( this->PosX - currentX ) < 0.4 && std::abs( this->PosY - currentY ) < 0.4 )
                {
                    switch( kovi )
                    {
                        case BALFEL : currentX -= ( irany == BALFEL ? LEPES : sideLength ); currentY -= ( irany == BALFEL ? LEPES : sideLength ); break;
                        case JOBBFEL : currentX += ( irany == JOBBFEL ? LEPES : sideLength ); currentY -= ( irany == JOBBFEL ? LEPES : sideLength ); break;
                        case BALLE : currentX -= ( irany == BALLE ? LEPES : sideLength ); currentY += ( irany == BALLE ? LEPES : sideLength ); break;
                        case JOBBLE : currentX += ( irany == JOBBLE ? LEPES : sideLength ); currentY += ( irany == JOBBLE ? LEPES : sideLength ); break;
                        
                    }
                    if( most != irany )
                        sideLength += LEPES;

                    ira kovik = ( most == irany ) ? irany : static_cast<ira>(static_cast<int>(most)+2%4);
                    most = kovi;
                    kovi = kovik;

                    if( currentX < 0 || currentY < 0 || currentX > max_palya_x || currentY > max_palya_y )
                    {
                        allapot = VEDEKEZ_MAS;
                        currentX = targetp->PosX;
                        currentY = targetp->PosY;
                    }
            
                }
                else
                {
                    res.push_back( moveShip( this->ItemId , currentX , currentY ) );
                }
            }
            break;
            case TAMAD :
            {
                //qDebug() << "Akit shootolok:" << QString::fromStdString(targetp->ItemPlayer);
                //qDebug() << " " << this->ItemId  <<" x:" << this->PosX <<" y:" << this->PosY<< " tX:" << targetp->PosX << " tY:"<< targetp->PosY ;
                if( std::abs( this->PosX - targetp->PosX ) < 1.9 && std::abs( this->PosY - targetp->PosY ) < 1.9 )
                {
                    res.push_back( Shoot( this->ItemId , targetp->ItemId , std::min(targetp->Units + 1, this->Units) ) );
                    allapot = NINCS;
                }
                else
                {
                   // qDebug() << " " << this->ItemId << " támad és move";
                    res.push_back( moveShip( this->ItemId , targetp->PosX , targetp->PosY ) );
                }
            }
            break;
        case NINCS:
            break;
        default:
            break;
        }
        //std::cout << *this<< " messages: ";
        //for( auto mes : res )
        //    std::cout << mes << std::endl;
        return res;
    }

};




template<class T>
std::ostream& operator<<(std::ostream& o , const item<T>& i )
{

    return o << "Id:" << i.ItemId <<
                " Type:" << num_to_what[i.What] <<
                " Pos(" << i.PosX << "," << i.PosY <<
                ") Unit:" << i.Units <<
                " Player:" << i.ItemPlayer <<
                " IncTime:" << i.IncTime << std::endl;
}

template<class T>
std::ostream& operator<<(std::ostream& o , const moveable_item<T>& i )
{

    return o << "Id:" << i.ItemId <<
                " Type:" << num_to_what[i.What] <<
                " Pos(" << i.PosX << "," << i.PosY <<
                ") Unit:" << i.Units <<
                " Player:" << i.ItemPlayer <<
                " currentX:" << i.currentX <<
                " currentY:" << i.currentY << std::endl;
}

template <typename T>
std::vector<moveable_item<>> tokenize( T buff , UL x , UL y)
{
    std::vector<moveable_item<>> res;
    std::istringstream is(buff);
    std::getline( is , buff , ':' );
    while( std::getline( is , buff , '|' ) )
    {
        if( !buff.size() ) break;

        res.push_back( moveable_item<>(buff ,x ,y) );
    }
    return res;
}

#endif // TOKENIZE_H

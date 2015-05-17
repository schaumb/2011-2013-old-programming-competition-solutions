#ifndef TOKENIZE_H
#define TOKENIZE_H
#include <istream>
#include <sstream>
#include <map>
#include <string>
#include <iterator>
#include <algorithm>
#include <vector>

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
    double targetX;
    double targetY;
    double sideLength;

    moveable_item( std::string buff = "" ): item<T>(buff){}

    bool areWeThere() {
        if(this->PosX == targetX && this->PosY == targetY) {
            return true;
        }
        return false;
    }

    void setDirection(double x, double y) {
        targetX = x;
        targetY = y;

    }

    void move() {
        if(!areWeThere()) {
            //this->PosX -= 1;
            qDebug() << this->PosY;
            this->PosY += 1;
            qDebug() << this->PosY;
        }
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

template <typename T>
std::vector<moveable_item<>> tokenize( T buff)
{
    std::vector<moveable_item<>> res;
    std::istringstream is(buff);
    std::getline( is , buff , ':' );
    while( std::getline( is , buff , '|' ) )
    {
        if( !buff.size() ) break;

        res.push_back( buff );
    }
    return res;
}

#endif // TOKENIZE_H

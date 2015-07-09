// stream includes
#include <iostream>
#include <fstream>
#include <sstream>

// data structures
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

// etc
#include <type_traits>
#include <memory>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <regex>

// kiszámolja egy számhoz egy másik számnál kisebb, legnagyobb osztóját
template<class T>
inline T GreatestDivisorTo(T value, T toDivisor)
{
    // ha nagyobbegyenlő az osztó, akkor az első osztó szám úgy is maga az érték lesz
    if(toDivisor >= value) return value;

    // amíg van osztási maradék, csökkentsük az értéket
    while(value % toDivisor) --toDivisor;
    return toDivisor;
}

int main()
{
    using Type = int; // milyen típusú számokkal számolunk.

    // létrehozunk a könnyebb beolvasás és inicializás, illetve konstansság megtartása érdekében egy bemeneti iterátort
    std::istream_iterator<Type> input(std::cin);

    // beolvassuk a bemenetet, majd meghatározzuk A-nak egy osztóját, mely legközelebb van az első koordináta maximumához
    const Type maxX = *input,
               maxY = *++input,
               valueOfA = *++input,
               greatestXDivisor = GreatestDivisorTo(valueOfA, maxX);

    // a kezdeti módszer az a következő:
    //   bontsuk fel egy megfelelő szorzatra A-t  = x*y
    //   majd képezzük a következő ponthármast:
    // 0 0
    // x 0
    // 0 y
    // ha megfelel a max határoknak, akkor készen is vagyunk
    if(valueOfA / greatestXDivisor <= maxY)
    {
        // kiiratjuk a fix koordinátákat, illetve a kiszámoltakat
        std::cout << "0 0 " << greatestXDivisor << " 0 0 " << valueOfA / greatestXDivisor << std::endl;
        return 0;
    }

    // ez sajna nem minden esetben működik (pl 4 4 7 esetén nem találja meg a 0 0 4 3 3 4 -t)
    // itt már brute-force algoritmust használunk egy darab fix (0,0) koordinátához
    // azzal az információval, hogy |x2*y3 - x3*y2| = A

    // a másik kettőnek a koordinátái:
    Type x2, y2,
         x3, y3;

    // tfh x2*y3 > x3*y2 (szimmetrika miatt)
    // végigmegyünk x2 lehetségesein
    for(x2 = maxX; x2; --x2)
    {
        // az ehhez tartozó y3-on is (figyelve: x2*y3 - A >= 0)
        Type remainingCoordsProd;
        for(y3 = maxY; (remainingCoordsProd = x2*y3 - valueOfA) >= 0; --y3)
        {
            // felbontjuk szorzattá a maradékot a segédfüggvényünkkel (kisebbegyenlő lesz x3 mint x2):
            x3 = GreatestDivisorTo(remainingCoordsProd, x2);

            // ha az y koordinátának is megfelel
            if((y2 = remainingCoordsProd / x3) <= maxY)
            {
                // akkor irassuk ki!
                std::cout << "0 0 " << x2 << " " << y2 << " " << x3 << " " << y3 << std::endl;
                return 0;
            }
        }
    }

    // nincs ilyen háromszög a megadott intervallumon belül
    std::cout << "0 0 0 0 0 0" << std::endl;

    return 0;
}

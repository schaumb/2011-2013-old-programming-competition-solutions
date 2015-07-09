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
#include <valarray>

// etc
#include <type_traits>
#include <memory>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <regex>

// eldönti egy számról, hogy csúnya-e
template<class T>
inline bool IsUgly(T num)
{
    // ha mindegyik egyjegyű számmal van osztási maradéka, akkor nem csúnya, kben igen
    return !(num % 2 && num % 3 && num % 5 && num % 7);
}

// ez egy általános N bázisú osztály
template<class Num, Num Base>
class NthBaseClass
{
    // ebben tárolódnak el a számjegyek
    std::valarray<Num> digitsRepr;

public:
    // konstruktor, megkapja, hogy maximálisan hány számjegyet tartalmazhat
    inline explicit NthBaseClass(Num maxDigit):digitsRepr(maxDigit){}

    // ++ operátor megnöveli a számrendszernek megfelelő módon az értékét
    inline NthBaseClass& operator++()
    {
        // addig megy a számjegyeken, amíg véget nem ér, vagy nem talál egy olyan számjegyet,
        // melynek a megnövelt értéke nem éri még el a bázist; ha elérte, akkor lenullázza
        for(auto digit = std::begin(digitsRepr); digit != std::end(digitsRepr) && ++*digit == Base; *digit++ = 0);
        return *this;
    }

    // lekérdezhető, hogy a 0-át reprezentáló szám-e
    inline bool IsZero() const
    {
        // a számok összegét megtagadva csak a 0-ra lesz igaz
        return !digitsRepr.sum();
    }

    // lekérhető az n. számjegye a számnak
    inline const Num& operator[](Num n) const
    {
        return digitsRepr[n];
    }

    // lekérdezhető a számjegyek száma
    inline decltype(digitsRepr.size()) size() const
    {
        return digitsRepr.size();
    }
};

// ez a függvény tokenizálja a megadott szöveget a megadott szabályok szerint
template<class Num, Num Base>
inline Num Tokenize(const std::string& s, const NthBaseClass<Num, Base>& rules)
{
    // a végleges számnak, és a ciklusváltozónak az inicializálása
    Num result = 0,
        i = 0;

    // minden körben eltárolunk egy előjelet, illetve az aktuális számot
    bool sign = false;
    Num prevNum = s[i] - '0';

    // végigmegyünk a szabályokon...
    while( i < rules.size() )
    {
        switch(rules[i])
        {
        case 0 :
            // ha 0-ás szabály van, azaz összeolvasztás szabálya van,
            // akkor eltoljuk 10-el az eddigi számjegyeket
            prevNum *= 10;
            break;
        case 1 :
            // ha 1-es szabály van (+), akkor beírjuk az előző számot
            // majd lenullázzuk, és az előjelet beállítjuk hamisra (pozitív)
            result += sign ? -prevNum : prevNum;
            prevNum = 0;
            sign = false;
            break;
        case 2 :
            // ha 2-es szabály van (-), akkor beírjuk az előző számot
            // majd lenullázzuk, és az előjelet beállítjuk igazra (negatív)
            result += sign ? -prevNum : prevNum;
            prevNum = 0;
            sign = true;
            break;
        }
        // hozzáadjuk a következő karaktert a számhoz
        prevNum += s[++i] - '0';
    }
    // az utolsó számot is hozzáadva visszatérünk az eredménnyel
    return result += sign ? -prevNum : prevNum;
}


int main()
{
    using Type = int; // milyen típusú számokkal számolunk.

    // beolvassuk az első sort, amiben a tesztesetek száma van
    std::string inputLine;
    std::getline(std::cin, inputLine);

    // végigmegyünk a teszteseteken, és kiiratjuk az eredményt
    std::generate_n(std::ostream_iterator<Type>(std::cout, "\n"), std::stoi(inputLine), [&inputLine]
    {
        // beolvassuk az aktuális tesztesetet
        std::getline(std::cin, inputLine);

        Type result = 0;

        // létrehozunk egy 3 bázisú, teszteset mérete - 1 számjegyű számot
        NthBaseClass<Type, 3> loopCounter(inputLine.size() - 1);

        do
        {
            // ha az aktuális szám tokenizálva csúnya, növeljük a számlálót
            if(IsUgly(Tokenize(inputLine, loopCounter)))
            {
                ++result;
            }
        } // addig megyünk, míg újra el nem érjük a 0-ás reprezentált számot.
        while(!(++loopCounter).IsZero());

        return result;
    });

    return 0;
}

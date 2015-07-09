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

int main()
{
    using Type = int; // milyen típusú számokkal számolunk.
    using ValVector = std::valarray<Type>;
    using ValMatrix = std::valarray<ValVector>;

    // beolvassuk az első sort, ő az üzenet
    std::string message;
    std::getline(std::cin, message);

    // kitöröljuk belőle a space-eket
    message.erase(std::remove(message.begin(), message.end(), ' '), message.end());

    // létrehozunk a könnyebb beolvasás és inicializás, illetve konstansság megtartása érdekében egy bemeneti iterátort
    std::istream_iterator<Type> input(std::cin);

    // beolvassuk a bemeneti dimenziószámot, ez alapján létrehozunk valarray-okat
    const Type dimension = *input;
    ValMatrix matrix(ValVector(dimension), dimension);

    // beolvassuk a mátrixot
    for(ValVector& token : matrix)
    {
        std::copy_n(++input, dimension, std::begin(token));
    }

    // ha a message nem pont dim -mel osztható, kibővítjük
    if(message.size() % dimension)
    {
        message.resize( (message.size() / dimension + 1) * dimension, 'A' );
    }

    // végigmegyünk a message-en dimension-ossával
    for(auto messageIterator = message.begin(); messageIterator < message.end(); messageIterator+=dimension)
    {
        // áttranszformáljuk egy vector<Type>-é a stringet, majd alkotunk belőle egy valarray-t, amivől kivonjuk 'A'-t
        // elkészítjük a szorzatmátrixot, majd összeadogatva és modulózva megkapjuk az eredményt, karakterként kiiratjuk
        for(const ValVector& token : static_cast<ValMatrix>( matrix *
                                                            ( ValVector( std::vector<Type>{ messageIterator,
                                                                                            messageIterator + dimension}.data() ,
                                                                         dimension
                                                                       ) -= 'A' )))
        {
            std::cout << static_cast<char>(token.sum() % 26 + 'A');
        }
    }
    std::cout << std::endl;

    return 0;
}

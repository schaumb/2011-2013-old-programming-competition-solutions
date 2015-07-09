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

int main()
{
    using Type = int; // milyen típusú számokkal számolunk.

    // temporális string, ebben tároljuk a beolvasott sort
    std::string tmp;

    // beolvasunk egy sort, stream objektumba tároljuk, majd feltöltjük értékekkel a vectort
    std::getline(std::cin, tmp);
    std::stringstream streamLine(tmp);
    std::vector<Type> firstVector{std::istream_iterator<Type>(streamLine), std::istream_iterator<Type>()};

    // a második vector-t csak simán a cin-ről olvassuk be
    std::vector<Type> secondVector{std::istream_iterator<Type>(std::cin), std::istream_iterator<Type>()};

    // a legjobb eset ha az összeszorzandó számok páronkénti összkülönbsége maximális, azaz az egyik növekvő, a másik csökkenő sorrendben van
    std::sort(std::begin(firstVector), std::end(firstVector));
    std::sort(std::begin(secondVector), std::end(secondVector), std::greater<Type>());

    // kiszámoljuk a skaláris szorzatukat, kiiratjuk
    std::cout << std::inner_product(std::begin(firstVector), std::end(firstVector),
                                    std::begin(secondVector), 0) << std::endl;
    return 0;
}

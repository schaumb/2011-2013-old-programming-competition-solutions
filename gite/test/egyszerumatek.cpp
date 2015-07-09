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

    // létrehozunk a könnyebb beolvasás és inicializás, illetve konstansság megtartása érdekében egy bemeneti iterátort
    std::istream_iterator<Type> input(std::cin);

    // beolvassuk a 3 számot, majd elvégezzük a polinom műveleteket
    const Type  x = *input,
                y = *++input,
                z = *++input,
                result = std::pow(x + std::sqrt(y), z);

    // kiiratni az utolsó 3 számjegyet fogjuk csak ami megfelel 1000 osztási maradékának
    std::cout << result % 1000 << std::endl;

    return 0;
}

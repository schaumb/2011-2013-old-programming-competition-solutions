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

// kiszámolja 2 szám legnagyobb közös osztóját
template<class T>
inline T GreatestCommonDivisor(T first, T second)
{
    for (;;)
    {
        if (first == 0) return second;
        second %= first;
        if (second == 0) return first;
        first %= second;
    }
}

// kiszámolja 2 szám legkisebb közös többszörösét
template<class T>
inline T LeastCommonMultiple(T first, T second)
{
    // a legnagyobb közös osztó kiszámolása szükséges
    T gcd = GreatestCommonDivisor(first, second);

    // ha ez nem nulla, akkor a számok szorzata, osztva a lnko-val megkapjuk az eredményt
    return gcd ? (first / gcd * second) : 0;
}

// kiszámolja 1 től 'to' ig a természetes számok összegét
template<class T>
inline T SumOfNaturals(T to)
{
    return to * (to + 1) / 2;
}

int main()
{
    using Type = int; // milyen típusú számokkal számolunk.

    // létrehozunk a könnyebb beolvasás és inicializás, illetve konstansság megtartása érdekében egy bemeneti iterátort
    std::istream_iterator<Type> input(std::cin);

    // a változókat inicializáljuk, kiszámoljuk a két szám lkkt-ét
    const Type firstNumber = *input,
               secondNumber = *++input,
               maxNumber = *++input - 1, // kivonunk egyet, mert az utolsó, beolvasott szám nincs benne a halmazba
               leastCommonMultipleNumber = LeastCommonMultiple(firstNumber, secondNumber);

    // a megoldás a számok szorozva a halmazba való beleférésükig (max / szám) a számok összege
    // hisz 1 * num + 2 * num + ... + (max/num) * num = num * (1 + 2 + ... + (max/num))
    // és ezek összegéből kivonva a közös többszöröseik értékét (hisz azt kétszer számoltuk
    std::cout << firstNumber * SumOfNaturals(maxNumber / firstNumber)
                 + secondNumber * SumOfNaturals(maxNumber / secondNumber)
                 - leastCommonMultipleNumber * SumOfNaturals(maxNumber / leastCommonMultipleNumber) << std::endl;
    return 0;
}

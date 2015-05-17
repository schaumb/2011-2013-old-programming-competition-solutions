
#include <string>
#include <vector>

struct X
{
    std::string in; // bemenet
    unsigned x; // hol tartunk
    unsigned szorzo; // ha szamot ismertunk fel, akkor mi volt az
    X(std::string in):in(in),x(0),szorzo(0){} // construktor
    enum nextToken{ ERR = -1, // error - lexikalis hiba
                    END = 0, // vegetoken
                    EGYI = 1 , // egy i-t ismerunk fel
                    KETI = 2, // ket i-t ismerunk fel
                    LHAMIS = 3, // l betut ism fel
                    IUJSOR = 4, // nagyit ism fel
                    PONT = 5, // pontot ism fel
                    SZAM = 6, // (nem1el kezdodo)szamot ism fel
                    SZAMRABLO = 7}; // 1el kezdodo szamot ism fel
    // tokenek
    nextToken hasNextToken();
    std::vector<int> calc();
};

std::vector<int> countDiamonds(std::string input);


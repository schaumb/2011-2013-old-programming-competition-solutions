
#include <iostream>
#include <string>
#include <algorithm>

#include "diamond.hpp"

X::nextToken X::hasNextToken()
{
    if(x == in.length()) return END; // vege -> vege

    { //PARSE
        switch(in[x])
        {
            case 'i' : // i-t olvasunk
            {
                ++x;
                if(x < in.length() && in[x] == 'i')
                { // ha kovetkezo is az, ket i lesz
                    ++x;
                    return KETI;
                }
                //kulonben csak egy
                return EGYI;
            };
            case 'I' : ++x; return IUJSOR; // I uj sor
            case 'l' :
            { // l - hamisgyemant
                ++x;
                // ha a kovi karakter i - o lesz a hamis atlepjuk
                if(x < in.length() && in[x] == 'i')
                    ++x;
                return LHAMIS;
            }
            case '.' : ++x; return PONT; // pont - uj scope
            case '1' :
            { // 1-essel kezdodo szam - rablo
                while(++x < in.length() &&
                      '0'<= in[x] &&
                      '9'>= in[x]);
                return SZAMRABLO;
            } ;
            case '0' :
            case '2' :
            case '3' :
            case '4' :
            case '5' :
            case '6' :
            case '7' :
            case '8' :
            case '9' :
            { // massal kezdodo szam - szorzo
                szorzo = 0;
                do{
                    szorzo*=10;
                    szorzo+=in[x]-'0';
                    ++x;
                }
                while(x < in.length() &&
                      '0'<= in[x] &&
                      '9'>= in[x]);
                return SZAM;
            };
            default : return ERR;
                // kulonben lexical error
        }
    }
}

std::vector<int> X::calc()
{
    nextToken n;
	std::vector<int> res;
    int db = 0;
    int scope = 0;
    while((n = hasNextToken()))
    {
        if(ERR == n)
        { // lexical error
            std::cout << "ERR : " << x << ". : " << in[x] << "\n";
            break;
        }

        switch(n)
        {
            case KETI : // ket i, noveljuk a scopeon beluli osszeget
            {
                ++scope;
                break;
            };
            case IUJSOR :
            { // uj sor, beirjuk res-be az eredmenyt, uj osszeget,
                // scopeot kezdunk
                db += scope;
				res.push_back(db);
                scope = 0;
                db = 0;
                break;
            };
            case SZAMRABLO :
            { // elrabolja a scope osszeget
                scope = 0;
                break;
            };
            case SZAM :
            {
                // megszorozzuk a scopeon beluli ossszeget, majd...
                scope *= szorzo;
            }
            case LHAMIS :
            case PONT :
            case EGYI :
            {
                // beirjuk a db-ba, majd uj scopeot nyitunk
                db += scope;
                scope = 0;
                break;
            }
            default :
                std::cout << "WTF ERR?!?!\n";
        }


    }
    // az utolso db-szamot is kiirjuk
    db += scope;
	res.push_back(db);
	return res;
}

std::vector<int> countDiamonds(std::string input) {
	//prepare input: remove \n and \r
	std::string::iterator end = std::remove_if(input.begin(), input.end(), [](char ch) { return ch == '\n' || ch == '\r'; });
	
	X uj(std::string(input.begin(), end));
	return uj.calc();
}


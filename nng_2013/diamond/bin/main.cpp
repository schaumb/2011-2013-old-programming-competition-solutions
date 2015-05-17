
#include <iostream>
#include <string>

#include "diamond.hpp"
#include "huffman.hpp"

int main(int argc, char **/*argv*/) {
	if ( argc > 1 ) {
		decode(read());
		return 0;
	}
    std::string be;
	std::string line;
	while ( std::getline(std::cin, line) ) {
		be += line + "\n";
	}
	for ( int x : countDiamonds(be) ) {
    	std::cout << x << std::endl;
	}
    return 0;
}



#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

int main() {

	std::ifstream in("g.in");

	int tcs;
	in >> tcs;

	in.ignore();

	for ( int ttt = 1; ttt <= tcs; ++ttt ) {
		char prev, next;
		std::map<char, int> db;
		std::map<char, char> apa;

		std::string line;
		std::getline(in, line);
		
		std::stringstream ss(line);

		ss >> prev;
		apa[prev] = prev;
		db[prev] = -1;

		while ( ss >> next ) {
			if ( prev == next ) {
				++db[prev];
				prev = apa[prev];
			} else {
				++db[prev];
				apa[next] = prev;
				db[next] = 0;
				prev = next;
			}
		}

		std::cout << "Case " << ttt << std::endl;
		for ( std::map<char, int>::iterator it = db.begin(); it != db.end(); ++it ) {
			std::cout << it->first << " = " << it->second << std::endl;


		}

	}

}

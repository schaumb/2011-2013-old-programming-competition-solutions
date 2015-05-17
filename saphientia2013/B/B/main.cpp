
#include <iostream>
#include <map>
#include <fstream>
#include <string>

int ass(int a) {
	if ( a < 0 ) {
		return -a;
	}
	return a;}

int main() {
	std::ifstream in("b.in");
	std::string s;
	int k;

	std::map<std::string, int> m;
	while ( in >> s >> k ) {
		if ( m.count(s) > 0 ) {
			if ( m[s] != -1 ) {
				if ( ass(m[s]-k) < 2 ) {
					std::cout << s << std::endl;
					m[s] = -1;
				} else {
					m[s] = k;
				}
			} 
		} else {
			m[s] = k;
		}
	}

}


#include <vector>
#include <iostream>
#include <fstream>
#include <map>

std::vector<std::vector<int> > v;
std::vector<int> apa;

int bejar(int i) {
	apa[i] = 1;
	int res = 1;
	for ( std::vector<int>::iterator it = v[i].begin(); it != v[i].end(); ++it ) {
		if ( apa[*it] == 0 ) {
			res += bejar(*it);
		}
	}
	return res;
}

int main() {
	std::ifstream in("e.in");
	int tcs;
	in >> tcs;

	while ( tcs-- ) {
		std::map<int, int> m;
		int db = 0;
		int num;
		in >> num;
		v.resize(num, std::vector<int>());

		apa.resize(num);

		int k;
		in >> k;


		
		for ( int i = 0; i < k; ++i ) {
			int xindex, yindex, x, y;
			in >> x >> y;
			if ( 0 == m.count(x) ) {
				m[x] = db++;
			}
			if ( 0 == m.count(y) ) {
				m[y] = db++;
			}
			v[m[x]].push_back(m[y]);
			v[m[y]].push_back(m[x]);
		}

		
		int opt = 0;

		for ( int i = 0; i < num; ++i ) {
			int res = 0;
			if ( apa[i] == 0 ) {
				res = bejar(i);
			}
			if ( res > opt ) {
				opt = res;
			}
		}

		std::cout << opt << std::endl;
		v.clear(); apa.clear();
	}
}

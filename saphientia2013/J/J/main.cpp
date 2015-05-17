
#include <vector>
#include <fstream>
#include <set>
#include <iostream>
#include <algorithm>

int main() {
	std::fstream in("j.in");

	int tcs;
	in >> tcs;

	while (tcs--) {
		int m,n;
		in >> m >> n;
		std::vector<std::vector<int> > v(m, std::vector<int>(n, 0));

		int db = 0;

		for ( int i = 0; i < m; ++i ) {
			for ( int j = 0; j < n; ++j ) {
				in >> v[i][j];
				
				db = std::max(db, v[i][j]);
				--v[i][j];
			}
		}

		std::vector<std::set<int> > sets(db);

		for ( int i = 0; i < m;  ++i ) {
			for ( int j = 0; j < n; ++j ) {
				if ( j != 0 && v[i][j] != v[i][j-1] ) {
					sets[v[i][j]].insert(v[i][j-1]);
					sets[v[i][j-1]].insert(v[i][j]);
				}
				if ( i != 0 && v[i][j] != v[i-1][j] ) {
					sets[v[i][j]].insert(v[i-1][j]);
					sets[v[i-1][j]].insert(v[i][j]);
				}
			}
		}

		std::vector<int> szin(db, 0);

		int szindb = 1;
		for ( int i = 0; i < db;  ++i ) {
			if ( szin[i] == 0 ) {
				szin[i] = szindb++;
				std::set<int> notSet = sets[i];
				notSet.insert(i);

				for ( int j = 0; j < db; ++j ) {
					if ( notSet.count(j) == 0  && szin[j] == 0 ) {
						szin[j] = szin[i];
						notSet.insert(j);
						notSet.insert(sets[j].begin(), sets[j].end());
					}
				}
			}
		}

		for ( int i = 0; i < m; ++i ) {
			for ( int j = 0; j < n; ++j ) {
				std::cout << szin[v[i][j]] << ' ';
			}
			std::cout << std::endl;
		}

		std::cout << std::endl;


	}

}

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <set>
#include <algorithm>

struct stargate {
	stargate(int x=0, int y=0, int z=0) : x(x), y(y), z(z) {}
	int x, y, z;

}
;
template<class T>
void print(const std::vector<std::vector<T> >& c) {
	for ( int x = 0; x < c.size(); ++x ) {
		for ( int y = 0; y < c[x].size(); ++y ) {
			std::cout << c[x][y] << '\t';
		}
		std::cout << std::endl;
	}
}

double distance(int ax, int ay, int az, int bx, int by, int bz) {
	double dx = ax-bx;
	double dy = ay-by;
	double dz = az-bz;
	//std::cout << dx << ' ' << dy << ' ' << dz << '\n';
	//std::cout << std::sqrt( dx*dx + dy*dy + dz*dz ) << '\n';
	return std::sqrt( dx*dx + dy*dy + dz*dz );
}

double distance(stargate a, stargate b) {
	return distance(a.x, a.y, a.z, b.x, b.y, b.z);
}

int round(double d) { 
	return std::floor(d+0.5);
}

int main() {

	//std::cout << distance(-6, -9, 0, 6,5,0) << std::endl;


	std::ifstream in("f.in");
	int tcs;
	in >> tcs;

	for ( int i = 0; i < tcs; ++i ) {

		std::vector<stargate> stargates;

		int s;
		in >> s;
		
		std::vector<std::vector<double> > distances(s+2, std::vector<double>(s+2, -1.0));

		for ( int j = 0; j < s; ++j ) {
			int x, y, z;
			in >> x >> y >> z;

			stargates.push_back(stargate(x,y,z));
			
			distances[j][j] = 0.0;
			for ( int k = 0; k < j; ++k ) {
				double d = distance(stargates[k], stargates[j]);
				distances[k][j] = distances[j][k] = d;
			}

			
		}
		
		int c;
		in >> c;

		std::vector< std::set<int> > clusters;

		for ( int k = 0; k < c; ++k ) {
			int i, j;
			in >> i >> j;

			distances[j-1][i-1] = distances[i-1][j-1] = 0.0;

		}

		int ax, ay, az;
		int bx, by, bz;

		in >> ax >> ay >> az;
		in >> bx >> by >> bz;

		stargates.push_back(stargate(ax,ay,az));
		stargates.push_back(stargate(bx,by,bz));
		
		distances[s][s] = 0.0;
		distances[s+1][s+1] = 0.0;

		for ( int k = s; k < s+2; ++k ) {
			for ( int l = 0; l < k; ++l ) {
				double d = distance(stargates[k], stargates[l]);
				distances[k][l] = distances[l][k] = d;
			}
		}

		std::vector<std::vector<double> >& distances2 = distances;

		for ( int i = 0; i < s+2; ++i ) {
			//print(distances);
			
			//std::cout << '\n';
		
			for ( int x = 0; x < s+2; ++x ) {
				for ( int y = 0; y < s+2; ++y ) {
					for ( int g = 0; g < s+2; ++g ) {
						if ( false && x == 0 && i == 0 ) {
							//std::cout << x << " " << y << " " << g << '\n';

							//std::cout << distances[x][g] << '+' << distances[g][y] << '\n';
							//std::cout << distances[x][g] + distances[g][y] << '\n';
							std::cout << distances[x][y] << '\n';
						}
						if ( distances[x][g] + distances[g][y] < distances[x][y] ) {
							//std::cout << x << " " << y << " " << g <<   " : volt: " << distances2[x][y] << " lett: " << distances[x][g] + distances[g][y] << std::endl;
							distances2[x][y] = distances[x][g] + distances[g][y];
						} else {
							distances2[x][y] = distances[x][y];
						}

						//distances2[x][y] = std::min(distances[x][g] + distances[g][y], distances[x][y]);
					}
				}
			}
			for ( int x = 0; x < s+2; ++x ) {
				for ( int y = 0; y < s+2; ++y ) {
					distances[x][y] = distances2[x][y];
				}
			}
			//distances = distances2;	
		}
		std::cout << round(distances2[s][s+1]) << std::endl;
	}

}

 


#include <fstream>
#include <utility>
#include <set>
#include <iostream>
#include <cmath>
struct harmas {

	harmas() {}

	harmas(int a, int b,int c) : a(a), b(b), c(c) {}

	int a,b,c;

	bool operator<(const harmas& o) const {
		return a < o.a;
	}

	
};

std::ostream& operator<<(std::ostream& os, const harmas& h ) 
{
	os << h.a << ' ' << h.b << ' ' << h.c;
	return os;
}

int main() {


	std::ifstream in("c.in");
	std::set<harmas> h;
	std::set<std::pair<int, int> > p;

	int a,b;
	while ( in >> a >> b ) {
		if ( b > a ) {
			//std::swap(a,b);
		}
		p.insert(std::make_pair(a,b));
		
	}

	for ( std::set<std::pair<int, int> >::iterator it = p.begin();it != p.end(); ++it) {
		int d = std::sqrt(double(it->first * it->first + it->second * it->second));
		if ( p.count(std::make_pair(a,d))  && 
			p.count(std::make_pair(b,d)) ) {

				h.insert(harmas(it->first, it->second, d));

		}
	}

	if ( h.size() > 0 ) {
	std::cout << h.size() << std::endl;
	std::cout << *h.begin() << std::endl;
	std::cout << *h.rbegin() << std::endl;
	} else { std::cout << "NO" << std::endl; }

	
}


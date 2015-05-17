
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

unsigned int m_w;
unsigned int m_z;
unsigned myrandom(unsigned m) {
	m_z = 36969*(m_z & 65535) + (m_z >> 16);
	m_w = 18000*(m_w & 65535) + (m_w >> 16);
	return ((m_z << 16) + m_w) % m;
}

typedef std::pair<unsigned, unsigned> pont;

int main() {
	std::ifstream in("a.in");
	unsigned tcs;
	in >> tcs;
	for ( int i = 0; i < tcs; ++i ) {
		m_w = 11;
		m_z = 173;

		std::set<pont> pontok;

		unsigned N, M;
		in >> M >> N;

		unsigned repc = 0;

		while ( pontok.size() < N ) {
			unsigned tmp = pontok.size();
			unsigned x = myrandom(M);unsigned y = myrandom(M);
			//std::cout << x << ", " << y << std::endl;
			pontok.insert(pont(x,y));
			repc += (pontok.size() == tmp);
		}
		std::cout << repc << std::endl;
	}

	//std::cin.ignore();
}

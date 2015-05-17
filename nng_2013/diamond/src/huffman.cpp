
#include "huffman.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <bitset>

std::vector<Byte> read() {
	std::ifstream in("gem2.encoded", std::ios::in | std::ios::binary);
	// get length of file:
	in.seekg (0, in.end);
	int length = in.tellg();
	in.seekg (0, in.beg);

	std::vector<Byte> data(length);

	std::cout << "Reading " << length << " characters... ";
	// read data as a block:
	in.read(reinterpret_cast<char *>(&data[0]), length);
	in.close();

	return data;

}

template<class T>
std::string binary(T data) {
	std::bitset<sizeof(T)*8> bs(data);
	std::stringstream ss;
	ss << bs;
	return ss.str();
}

typedef std::vector<bool> Bits;
bool tryMatch(const Bits& data, const Bits& pattern, unsigned offset) {
	if ( pattern.size() + offset >= data.size() ) {
		return false;
	}
	for ( unsigned i = 0; i < pattern.size(); ++i ) {
		if ( data[offset + i] != pattern[i] ) {
			return false;
		}
	}
	return true;
}

void decode(const std::vector<Byte>& data) {
	//int codedchars = 10000000; // first 4 bytes
	Bits bits;
	for ( unsigned i = 4; i < data.size(); ++i ) {
		for ( int j = 0; j < 8; ++j ) {
			bits.push_back( (data[i] & (1 << j)) != 0 );
		}
	}

	std::vector< std::pair< Bits, char > > map;
	map.push_back( std::make_pair( Bits({1}), 'i' ) );
	map.push_back( std::make_pair( Bits({0, 0}), '.' ) );
	map.push_back( std::make_pair( Bits({0, 1, 1, 1}), 'l' ) );
	map.push_back( std::make_pair( Bits({0, 1, 0, 0, 1}), '1' ) );
	map.push_back( std::make_pair( Bits({0, 1, 0, 1, 1, 0}), '0' ) );
	map.push_back( std::make_pair( Bits({0, 1, 1, 0, 1, 1}), '2' ) );
	map.push_back( std::make_pair( Bits({0, 1, 1, 0, 0, 0}), '3' ) );
	map.push_back( std::make_pair( Bits({0, 1, 0, 1, 1, 1}), '4' ) );
	map.push_back( std::make_pair( Bits({0, 1, 0, 1, 0, 1}), '5' ) );
	map.push_back( std::make_pair( Bits({0, 1, 1, 0, 0, 1}), '6' ) );
	map.push_back( std::make_pair( Bits({0, 1, 0, 1, 0, 0}), '7' ) );
	map.push_back( std::make_pair( Bits({0, 1, 0, 0, 0, 1}), '8' ) );
	map.push_back( std::make_pair( Bits({0, 1, 1, 0, 1, 0}), '9' ) );
	map.push_back( std::make_pair( Bits({0, 1, 0, 0, 0, 0, 1}), ' ' ) );
	map.push_back( std::make_pair( Bits({0, 1, 0, 0, 0, 0, 0}), 'I' ) );

	for ( unsigned i = 0; i < bits.size(); ) {
		bool found = false;
		for ( unsigned j = 0; j < map.size(); ++j ) {
			if ( tryMatch(bits, map[j].first, i) ) {
				std::cout << map[j].second;
				found = true;
				i += map[j].first.size();
				break;
			}
		}
		if ( !found ) {
			std::cout << "ERRRRRORRRRR" << std::endl;
			break;
		}
	}
	std::cout << std::endl;
}



#include "Map.h"
#include "parseWithStream.h"

#include <iostream>
#include "stdafx.h"

Field charToField(char ch) {
	if ( ch == '.' ) {
		return NOWALL;
	}
	if ( ch == '#' ) {
		return WALL;
	}
	std::cerr << "Invalid field char '" << ch << "'" << std::endl;
	return WALL;
}

char fieldToChar(Field f) {
	if ( f == WALL ) { return '#'; }
	if ( f == NOWALL ) { return ' '; }
	assert(false);
	return '?';
}

bool Map::updateMap(const std::vector<std::string>& lines) {

	std::string temp;
	std::string statusString;
	
	int i = 0;
	parseWithStream(lines[i++], statusString);	//ok,start,timeout

	bool isNewMap = statusString == "start";

	parseWithStream(lines[i++], temp, tick); //tick 38
	assert(temp == "tick");

	std::cout << "line : " << lines[i] << std::endl;
	parseWithStream(lines[i++], temp, width, height); //map h w
	std::cout << "w: " << width << std::endl;
	assert(temp == "map");

	if ( isNewMap ) {
		fields = FieldMatrix(height, std::vector<Field>(width, WALL));
		for ( int y = 0; y < height; ++y ) {
			for ( int x = 0; x < width; ++x ) {
				fields[y][x] = charToField(lines[i+y][x]);
			}
		}
	}

	i += height;

	gems = GemMatrix(height, std::vector<int>(width, 0));
	players = PlayerMatrix(height, std::vector<Player>(width));
	


	parseWithStream(lines[i++], temp, myId, myX, myY, myScore);
	assert(temp == "state");
	
	parseWithStream(lines[i++], temp, gemCount);
	assert(temp == "gems");

	for ( int j = 0; j < gemCount; ++j ) {
		int gemx, gemy, gemvalue;
		parseWithStream(lines[i+j], gemx, gemy, gemvalue);
		gems[gemy][gemx] = gemvalue;
	}
	i += gemCount;

	parseWithStream(lines[i++], temp, playerCount);
	assert(temp == "players");

	for ( int j = 0; j < playerCount; ++j ) {
		std::string id;
		int playerx, playery, playerScore;
		parseWithStream(lines[i+j], id, playerx, playery, playerScore);
		players[playery][playerx] = Player(id, playerScore);
	}
	i += playerCount;
	assert(lines[i] == ".");

	return isNewMap;
}

std::string Map::toString() const {
	std::stringstream ss;
	ss << "Map: w: " << width << " h:" << height << "\n";
	for ( int y = 0; y < height; ++y ) {
		for ( int x = 0; x < width; ++x ) {
			ss << fieldToChar(fields[y][x]);
		}
		ss << '\n';
	}
	return ss.str();
}



#ifndef MAP_H_
#define MAP_H_

#include "stdafx.h"

//template<class T>
//using Matrix = std::vector<std::vector<T> >;


enum Field { WALL, NOWALL };

struct Player {
	Player() : id(), score() {}
	Player(const std::string& id, int score) : id(id), score(score) {}
	std::string id;
	int score;
};


typedef std::vector<std::vector<Field> > FieldMatrix;
typedef std::vector<std::vector<int> > GemMatrix;
typedef std::vector<std::vector<Player> > PlayerMatrix;

class Map {
public:

	int tick;
	int width, height;
	std::string myId;

	int myY, myX;
	int myScore;
	int gemCount;
	int playerCount;

	bool updateMap(const std::vector<std::string>& lines);

	Field getXYField(int x, int y) {
		return fields[y][x];
	}
	int getXYGem(int x, int y) {
		return gems[y][x];
	}
	Player getXYPlayer(int x, int y) {
		return players[y][x];
	}

	//m[sor][oszlop]
	FieldMatrix fields;
	GemMatrix gems;
	PlayerMatrix players;

	std::string toString() const;

};

#endif

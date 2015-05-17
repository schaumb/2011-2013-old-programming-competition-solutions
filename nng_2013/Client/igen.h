
#ifndef IGEN_H_
#define IGEN_H_

#include <map>
#include <deque>
#include <vector>
#include <algorithm>
#include <utility>
#include "Map.h"


enum class Direction{ North, West, South, East, Stay };
typedef std::pair<int, int> Point;

inline
std::string DirectionToString(Direction d) {
	switch(d) {
		case Direction::North: return "north";
		case Direction::West: return "west";
		case Direction::South: return "south";
		case Direction::East: return "east";
		case Direction::Stay: return "stay";
	}
	return "wtf";
}

typedef std::deque<Direction> Route;

inline
bool isFree(int y, int x, const Map& m) {
        return m.fields[y][x] == Field::NOWALL;

}

inline
bool notYetVisited(int y, int x, std::vector<std::vector<Point> > parents) {
        return parents[y][x] == std::make_pair(-1, -1);
}

inline
Direction getDelta(Point from, Point to) {
        if(from.first < to.first) {
                return Direction::North;
        }
        if(from.first > to.first) {
                return Direction::South;
        }
        if(from.second < to.second) {
                return Direction::West;
        }
        if(from.second > to.second) {
                return Direction::East;
        }
        
        return Direction::Stay;
}

inline
Route getRouteToNextGem(const Map& map) {
        const auto& startX = map.myX;
        const auto& startY = map.myY;

        std::deque<Point> visited;
        visited.emplace_back(startY, startX);

        std::vector<std::vector<Point> > parents(map.width, std::vector<Point>(map.height, std::pair<int, int>(-1, -1)));
        parents[startY][startX] = std::make_pair(startY, startX);
        while(!visited.empty()) {

                const auto currentPos = visited.front();

                if(map.gems[currentPos.first][currentPos.second] >0) {
                        // found closest gem.
                        break;
                }

                visited.pop_front();
                
                // up
                {
                        const auto upperY = currentPos.first -1;
                        const auto upperX = currentPos.second;
                        if(isFree(upperY, upperX, map) && notYetVisited(upperY, upperX, parents) ) {
                                parents[upperY][upperX] = currentPos;
                                visited.emplace_back(upperY, upperX);
                        }
                }
                // down
                {
                        const auto lowerY = currentPos.first +1;
                        const auto lowerX = currentPos.second;
                        if(isFree(lowerY, lowerX, map) && notYetVisited(lowerY, lowerX, parents) ) {
                                parents[lowerY][lowerX] = currentPos;
                                visited.emplace_back(lowerY, lowerX);
                        }
                }
                // right
                {
                        const auto rightY = currentPos.first;
                        const auto rightX = currentPos.second +1;
                        if(isFree(rightY, rightX, map) && notYetVisited(rightY, rightX, parents) ) {
                                parents[rightY][rightX] = currentPos;
                                visited.emplace_back(rightY, rightX);
                        }
                }
                // left
                {
                        const auto leftY = currentPos.first;
                        const auto leftX = currentPos.second -1;
                        if(isFree(leftY, leftX, map) && notYetVisited(leftY, leftX, parents) ) {
                                parents[leftY][leftX] = currentPos;
                                visited.emplace_back(leftY, leftX);
                        }
                }


        }

        Route generatedRoute;


        for(Point tmp = visited.front(); tmp != parents[tmp.first][tmp.second]; tmp = parents[tmp.first][tmp.second]) {
                generatedRoute.push_front(getDelta(tmp, parents[tmp.first][tmp.second]));
        }

        return generatedRoute;
        
}

#endif IGEN_H_



//
// Created by Maschinr on 24.11.2018.
//

#ifndef SDL_PATHFINDING_PATHFINDER_H
#define SDL_PATHFINDING_PATHFINDER_H
#include <vector>
#include <stack>
#include "NavTile.h"

class Pathfinder {
public:
    ~Pathfinder();
	static Pathfinder& getInstance();
    std::stack<std::shared_ptr<Tile> > getPath(const Position&, const Position&);
    std::shared_ptr<Tile> getTile(const unsigned int, const unsigned int);
	std::shared_ptr<Tile> getTile(const Position&);
    const void generateNavMap(const unsigned int&, const unsigned int&);
	const size_t getWidth();
	const size_t getHeight(const unsigned int&);

private:
    Pathfinder();
	Pathfinder(const Pathfinder&);
    static Pathfinder* instance;
    std::vector<std::vector<std::shared_ptr<NavTile> > > nav_map;
    std::vector<std::shared_ptr<NavTile>> getAdjacentTiles(const std::shared_ptr<NavTile>&);
    unsigned int getDistance(const std::shared_ptr<NavTile>&, const std::shared_ptr<NavTile>&);
};


#endif //SDL_PATHFINDING_PATHFINDER_H

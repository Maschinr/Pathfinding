#ifndef SDL_PATHFINDING_NAVTILE_H
#define SDL_PATHFINDING_NAVTILE_H
#include "Tile.h"

class NavTile : public Tile {
public:
    NavTile(BLOCK_STATUS status, Position position, SDL_Rect rectangle);
    std::shared_ptr<NavTile> parent;
    unsigned int cost;
    unsigned int distance;
};


#endif //SDL_PATHFINDING_NAVTILE_H

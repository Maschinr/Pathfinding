#ifndef SDL_PATHFINDING_TILE_H
#define SDL_PATHFINDING_TILE_H

#include <SDL.h>
#include <memory>

enum BLOCK_STATUS {
    HARD_BLOCKED,
    SOFT_BLOCKED,
    FREE
};

typedef struct Position {
	unsigned int x;
    unsigned int y;
    Position() : x(0), y(0) { }
    Position(unsigned int x, unsigned int y) : x(x), y(y) { }
} Position;


class Tile {
public:
    Tile(BLOCK_STATUS status, Position position, SDL_Rect rectangle);

	void setStatus(const BLOCK_STATUS&);
	const BLOCK_STATUS& getStatus();
	
	const SDL_Rect& getRectangle();

	const Position& getPosition();

private:
    SDL_Rect rectangle;
    BLOCK_STATUS status;
    Position position;
};


#endif //SDL_PATHFINDING_TILE_H

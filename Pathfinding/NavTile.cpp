#include "NavTile.h"

NavTile::NavTile(BLOCK_STATUS status, Position position, SDL_Rect rectangle) : Tile(status, position, rectangle),  cost(0), distance(0) {

}


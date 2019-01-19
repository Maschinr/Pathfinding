#include "Tile.h"

Tile::Tile(BLOCK_STATUS status, Position position, SDL_Rect rectangle) : status(status), position(position), rectangle(rectangle) { }

void Tile::setStatus(const BLOCK_STATUS& status)
{
	this->status = status;
}

const BLOCK_STATUS& Tile::getStatus()
{
	return this->status;
}

const SDL_Rect& Tile::getRectangle()
{
	return this->rectangle;
}

const Position & Tile::getPosition()
{
	return this->position;
}





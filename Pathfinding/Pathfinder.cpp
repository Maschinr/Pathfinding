#include "Pathfinder.h"
#include <iostream>
#include <algorithm>

using namespace std;

Pathfinder* Pathfinder::instance;

Pathfinder::Pathfinder() : nav_map() 
{

}

Pathfinder::~Pathfinder() 
{
    delete instance;
}

Pathfinder& Pathfinder::getInstance() 
{
    if(instance == nullptr) {
        instance = new Pathfinder();
    }

    return *instance;
}

stack<shared_ptr<Tile> > Pathfinder::getPath(const Position& startPos, const Position& targetPos) {
    vector<shared_ptr<NavTile> > opened;
    vector<shared_ptr<NavTile> > closed;

	shared_ptr<NavTile> start = this->nav_map[startPos.x][startPos.y];
	shared_ptr<NavTile> target = this->nav_map[targetPos.x][targetPos.y];

    closed.push_back(start);

    vector<shared_ptr<NavTile> > adj = this->getAdjacentTiles(start);

    for(shared_ptr<NavTile> t : adj) {
        t->cost = this->getDistance(start, t);
        t->distance = this->getDistance(t, target);
    }
    opened.insert(end(opened), begin(adj), end(adj));


    //Start algorithm
    bool found = false;
	shared_ptr<NavTile> best;
    while(found == false) {
        auto min = min_element(opened.begin(), opened.end(),
                                    [](const shared_ptr<NavTile>& a, const shared_ptr<NavTile>& b) {
                                        return (a->cost + a->distance) < (b->cost + b->distance);
                                    });
        size_t index = distance(begin(opened), min);
        best = opened[index];
        opened.erase(opened.begin() + index);
        closed.push_back(best);

        adj = this->getAdjacentTiles(best);
        if(find(adj.begin(), adj.end(), target) != adj.end()) { // if target is adjacent
            target->parent = best;
            closed.push_back(target);
        } else {
            for (shared_ptr<NavTile> t : adj) {
                if (find(closed.begin(), closed.end(), t) == closed.end()) { // its not in the closed list
                    if (find(opened.begin(), opened.end(), t) == opened.end()) {
                        t->cost = this->getDistance(start, t);
                        t->distance = this->getDistance(t, target);
                        t->parent = best;
                        opened.push_back(t);
                    } else {
                        if (t->cost + t->distance < best->cost + best->distance) { // is it better
                            t->cost = this->getDistance(start, t);
                            t->distance = this->getDistance(t, target);
                            t->parent = best;
                        }
                    }
                }
            }
        }

        if(find(closed.begin(), closed.end(), target) != closed.end()) {
            found = true;
        } else if(opened.empty()) {
			break;
        }
    }

    if(found == true) {    //Generate path
        stack<shared_ptr<Tile> > path;

        path.push(target);

		shared_ptr<NavTile> cur = target;
        do {
            cur = cur->parent;
            if(cur != nullptr) {
                path.push(cur);
            }
        } while(cur != nullptr);

        return path;
    } else {
        return stack<shared_ptr<Tile> >();
    }


}

shared_ptr<Tile> Pathfinder::getTile(const unsigned int x, const unsigned int y)
{
	if (x < this->nav_map.size()) {
		if (y < this->nav_map[x].size()) {
			return this->nav_map[x][y];
		}
	}

	return nullptr;
}

shared_ptr<Tile> Pathfinder::getTile(const Position& position)
{
	return this->getTile(position.x, position.y);
}

const void Pathfinder::generateNavMap(const unsigned int& width, const unsigned int& height)
{
	this->nav_map.clear();
	
    for(size_t x = 0; x < width; x++) {
        this->nav_map.push_back(vector<shared_ptr<NavTile> >());
        for(size_t y = 0; y < height; y++) {
			SDL_Rect rectangle;
			rectangle.w = 25;
			rectangle.h = 25;
			rectangle.x = x * 25 + x; //remove +x/y to get without grid
			rectangle.y = y * 25 + y;
            this->nav_map[x].push_back(make_shared<NavTile>(FREE, Position(x, y), rectangle));
        }
    }
}

const unsigned int Pathfinder::getWidth()
{
	return this->nav_map.size();
}

const unsigned int Pathfinder::getHeight(const unsigned int& posX)
{
	if (this->nav_map.size() > posX) {
		return this->nav_map.at(posX).size();
	} else {
		throw "Invalid X position: " + posX;
	}
}

vector<shared_ptr<NavTile> > Pathfinder::getAdjacentTiles(const shared_ptr<NavTile>& tile)
{
    vector<shared_ptr<NavTile> > result;
	const Position& pos = tile->getPosition();

    if(pos.x + 1 < this->nav_map.size()) { //Right
		if (this->nav_map[pos.x + 1].size() > pos.y) {
			if (this->nav_map[pos.x + 1][pos.y]->getStatus() != HARD_BLOCKED) {
				result.push_back(this->nav_map[pos.x + 1][pos.y]);
			}
		}
       
    }

    if(pos.x != 0) { //Left
		if (this->nav_map[pos.x - 1].size() > pos.y) {
			if (this->nav_map[pos.x - 1][pos.y]->getStatus() != HARD_BLOCKED) {
				result.push_back(this->nav_map[pos.x - 1][pos.y]);
			}
		}
    }

    if(pos.y + 1 < this->nav_map[pos.x].size()) { // Bottom
        if(this->nav_map[pos.x][pos.y + 1]->getStatus() != HARD_BLOCKED) {
            result.push_back(this->nav_map[pos.x][pos.y + 1]);
        }
    }

    if(pos.y != 0) { // Top
        if(this->nav_map[pos.x][pos.y - 1]->getStatus() != HARD_BLOCKED) {
            result.push_back(this->nav_map[pos.x][pos.y - 1]);
        }
    }

    return result;
}

unsigned int Pathfinder::getDistance(const shared_ptr<NavTile>& t1, const shared_ptr<NavTile>& t2) {
    return (unsigned int)(t1->getPosition().x - t2->getPosition().x) + (unsigned int)(t1->getPosition().y - t2->getPosition().y);
}

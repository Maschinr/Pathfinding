#include "Game.h"
#include <iostream>

using namespace std;

Game::Game() : event(), window(nullptr, SDL_DestroyWindow), renderer(nullptr, SDL_DestroyRenderer), hoveringTile(nullptr), oldHoveringTile(nullptr)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		this->window.reset(SDL_CreateWindow("Title", 100, 100, 830, 830, NULL));
		if (window.get() != nullptr) {
			this->renderer.reset(SDL_CreateRenderer(this->window.get(), -1, NULL));
			if (renderer != nullptr) {
				this->running = true;
				Pathfinder::getInstance().generateNavMap(32, 32);
			} else {
				throw "Could not create Renderer";
			}
		} else {
			throw "Could not create Window";
		}
	} else {
		throw" Could not Initialize SDL";
	}
}

Game::~Game() 
{
    SDL_Quit();
}

void Game::loop() 
{
    while(this->running) {
        this->update();
        this->render();
    }
}

void Game::update() 
{
    while(SDL_PollEvent(&this->event) != 0) {
        if(event.type == SDL_QUIT) {
            this->running = false;
        } else if(this->event.type == SDL_MOUSEBUTTONDOWN) {
            if(this->event.button.button == SDL_BUTTON_LEFT) {
				this->hoveringTile = this->getHoverTile();
				if (this->hoveringTile != nullptr) {
					for (size_t x = 0; x < Pathfinder::getInstance().getWidth(); x++) {
						for (size_t y = 0; y < Pathfinder::getInstance().getHeight(x); y++) {
							shared_ptr<Tile> curTile = Pathfinder::getInstance().getTile(x, y);
							if (curTile->getStatus() == SOFT_BLOCKED) {
								curTile->setStatus(FREE);
							}
						}
					}


					stack<shared_ptr<Tile>> path = Pathfinder::getInstance().getPath(Position(0, 0), this->hoveringTile->getPosition());

					if (!path.empty()) {
						Pathfinder::getInstance().getTile(Position(0, 0))->setStatus(SOFT_BLOCKED);
						while (!path.empty()) {
							shared_ptr<Tile> t = path.top();
							Pathfinder::getInstance().getTile(t->getPosition())->setStatus(SOFT_BLOCKED);
							path.pop();
						}
					}
                }
			}
        }
    }

    if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) { // for continous input
		this->hoveringTile = this->getHoverTile();
		if (oldHoveringTile != this->hoveringTile && this->hoveringTile != nullptr) {
			if (this->hoveringTile->getStatus() == HARD_BLOCKED) {
				this->hoveringTile->setStatus(FREE);
			}
			else {
				this->hoveringTile->setStatus(HARD_BLOCKED);
			}
			this->oldHoveringTile = this->hoveringTile;
		}
    }

	
}

void Game::render() 
{
    SDL_SetRenderDrawColor(this->renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(this->renderer.get());

    for(size_t x = 0; x < Pathfinder::getInstance().getWidth(); x++) {
        for(size_t y = 0; y < Pathfinder::getInstance().getHeight(x); y++) {
			shared_ptr<Tile> curTile = Pathfinder::getInstance().getTile(x, y);
            if(curTile->getStatus() == FREE) {
                SDL_SetRenderDrawColor(this->renderer.get(), 0, 255, 0, 255);
            } else if(curTile->getStatus() == SOFT_BLOCKED) {
                SDL_SetRenderDrawColor(this->renderer.get(), 0, 0, 255, 255);
            } else if(curTile->getStatus() == HARD_BLOCKED) {
                SDL_SetRenderDrawColor(this->renderer.get(), 255, 0, 0, 255);
            }
            SDL_RenderFillRect(this->renderer.get(), &curTile->getRectangle());
        }
    }

    SDL_RenderPresent(this->renderer.get());
}

shared_ptr<Tile> Game::getHoverTile()
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	for (size_t x = 0; x < Pathfinder::getInstance().getWidth(); x++) {
		shared_ptr<Tile> cur_tile_x = Pathfinder::getInstance().getTile(x, 0);
		const SDL_Rect& cur_rect_x = cur_tile_x->getRectangle();

		if (cur_rect_x.x <= mouseX && cur_rect_x.x + cur_rect_x.w >= mouseX) { // It's the right column
			for (size_t y = 0; y < Pathfinder::getInstance().getHeight(x); y++) {
				shared_ptr<Tile> cur_tile_y = Pathfinder::getInstance().getTile(x, y);
				const SDL_Rect& cur_rect_y = cur_tile_y->getRectangle();

				if (cur_rect_y.y <= mouseY && cur_rect_y.y + cur_rect_y.h >= mouseY) {
					return Pathfinder::getInstance().getTile(x, y);
				}
			}
			break;
		}
	}

	return nullptr;
}



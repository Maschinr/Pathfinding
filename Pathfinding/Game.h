#ifndef SDL_PATHFINDING_GAME_H
#define SDL_PATHFINDING_GAME_H
#include <SDL.h>
#include "Pathfinder.h"
#include <memory>

class Game {
public:
    Game();
    ~Game();

    void loop();
    void update();
    void render();
private:
	//Miscellaneous
	bool running;
	std::shared_ptr<Tile> getHoverTile();
	Game(const Game&);

	//SDL Variables
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
    std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
	SDL_Event event;

	//Pathfinding Variables
	std::shared_ptr<Tile> hoveringTile;
	std::shared_ptr<Tile> oldHoveringTile;
};


#endif //SDL_PATHFINDING_GAME_H

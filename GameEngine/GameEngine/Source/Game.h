#pragma once

//#ifndef Game.h
//#define Game.h
//#define SDL_MAIN_HANDLED

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include <cstdlib>

class AssetManager;
class ColliderComponent;

class Game 
{
private:
	int cnt;
	int spawnSpeed = 289;
	int shieldTimer = 1440;
	SDL_Window* window;
	const static int gameSize = 960;
	int i = 0;
	static bool shield;

public:

	Game();
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void createProjectile(float x, float y, float xS, float yS, int r, int s);
	void createShield();
	int rng() { return (rand() % 10) + 1; }
	float getSpeed();
	void clean();
	bool running() { return isRunning; }
	static bool getShield() { return shield; }
	static void setShield(bool set) { shield = set; }

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static bool isRunning;
	static SDL_Rect camera;
	static AssetManager* assets;
	static int getGameSize() { return gameSize; };

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupShields
	};
};
//#endif // !Game.h

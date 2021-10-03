#pragma once

#include "Game.h"

class GameObject {

private:
	int xpos;
	int ypos;

	SDL_Texture* objTexture;
	SDL_Rect srcR, destR;

public:
	GameObject(const char* textureSheet, int x, int y);
	~GameObject();

	void Update();
	void Render();

};
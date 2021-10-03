#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* textureSheet, int x, int y)
{
	objTexture = TextureManager::LoadTexture(textureSheet);

	xpos = x;
	ypos = y;
}

void GameObject::Update()
{
	xpos++;
	ypos++;

	srcR.h = 32;
	srcR.w = 32;
	srcR.x = 0;
	srcR.y = 0;

	destR.x = xpos;
	destR.y = ypos;
	destR.w = srcR.w;
	destR.h = srcR.h;
}

void GameObject::Render()
{
	SDL_RenderCopy(Game::renderer, objTexture, &srcR, &destR);
}

GameObject::~GameObject()
{

}
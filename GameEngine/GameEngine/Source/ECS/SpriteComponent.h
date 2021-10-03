#pragma once

#include "Components.h"
#include "SDL.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"

class SpriteComponent : public Component
{
private:
	TransformComponent *transform;
	SDL_Texture* texture;
	SDL_Rect srcR, destR;

	bool animated = false;
	int frames = 0;
	int speed = 100;

public:

	int animIndex = 0;
	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTexture(id);
	}

	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;

		Animation idle = Animation(0, 3, 200);
		Animation walk = Animation(1, 3, 200);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);

		Play("Idle");

		setTexture(id);
	}

	~SpriteComponent()
	{
	}

	void setTexture(std::string id)
	{
		texture = Game::assets->GetTexture(id);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcR.x = srcR.y = 0;
		srcR.w = transform->width;
		srcR.h = transform->height;
	}

	void update() override
	{
		if (animated)
		{
			srcR.x = srcR.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcR.y = animIndex * transform->height;

		destR.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destR.y = static_cast<int>(transform->position.y)- Game::camera.y;
		destR.w = transform->width * transform->scale;
		destR.h = transform->height * transform->scale;
	}
	
	void draw() override
	{
		TextureManager::Draw(texture, srcR, destR, spriteFlip);
	}

	void Play(const char* animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};

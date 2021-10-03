#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component {
public:
	ProjectileComponent(int r, int s, Vector2D vel) : range(r), speed(s), velocity(vel){}
	~ProjectileComponent() {}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void update() override {
		distance += speed;

		if (distance > range) {
			//entity->destroy();
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w ||
			transform->position.x < Game::camera.x ||
			transform->position.y > Game::camera.y + Game::camera.h ||
			transform->position.y < Game::camera.y) {
			//entity->destroy();
		}
	}

private:
	TransformComponent* transform;

	int range;
	int speed;
	int distance;
	Vector2D velocity;
};
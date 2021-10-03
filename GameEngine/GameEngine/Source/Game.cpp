#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"

Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, Game::gameSize, Game::gameSize };

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;
bool Game::shield = false;

auto& player(manager.addEntity());
//auto& theShield(manager.addEntity());

Game::Game()
{}

Game::~Game()
{}


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	
	if (fullscreen) 
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		//System initialized
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}

	assets->AddTexture("terrain", "assets/terrain_ss.png");
	assets->AddTexture("player", "assets/mainchar_anims.png");
	assets->AddTexture("projectile", "assets/proj.png");
	assets->AddTexture("shield", "assets/shield.png");

	map = new Map("terrain", 3, 32);

	map->LoadMap("assets/map.map", 10, 10);

	player.addComponent<TransformComponent>(2);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player");
	player.addGroup(groupPlayers);
	/*
	theShield.addComponent<TransformComponent>(3);
	theShield.addComponent<SpriteComponent>("shield", false);
	theShield.addComponent<ColliderComponent>("shield");
	theShield.addGroup(groupShields);*/

	//assets->CreateProjectile(Vector2D(500, 500), Vector2D(0.2f, 0), 1000, 2, "projectile");

	//assets->CreateProjectile(Vector2D(500, 700), Vector2D(0.2f, 0), 1000, 2, "projectile");

	//assets->CreateProjectile(Vector2D(50, 50), Vector2D(0.5f, 0.5f), 1000, 2, "projectile");
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& shields(manager.getGroup(Game::groupShields));

void Game::handleEvents()
{

	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}

void Game::update()
{
	//std::cout << rng() << std::endl;
	std::cout << shieldTimer++ << std::endl;
	std::cout << i++ << std::endl;

	if (i == 1) {
		int random = rng();
		float randomSpeedX = getSpeed();
		float randomSpeedY = getSpeed();
		if (random <= 3)
			createProjectile(100.0f, 100.0f, randomSpeedX, randomSpeedY, 1000, 1);
		if (random >= 8)
			createProjectile(800.0f, 100.0f, -randomSpeedX, randomSpeedY, 1000, 1);
		if (random == 4 || random == 5)
			createProjectile(100.0f, 800.0f, randomSpeedX, -randomSpeedY, 1000, 1);
		if (random == 6 || random == 7)
			createProjectile(800.0f, 800.0f, -randomSpeedX, -randomSpeedY, 1000, 1);
	}

	if (i == spawnSpeed) {
		int random = rng();
		if (random <= 3 && spawnSpeed > 46)
			spawnSpeed -= 10;
		i = 0;
	}

	SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
	Vector2D playerPos = player.getComponent<TransformComponent>().position;
	
	if (shieldTimer >= 1440 && getShield() == false) {
		createShield();
		setShield(true);
		shieldTimer = 0;
	}
	else if (shieldTimer > 1440) {
		shieldTimer = 860;
	}

	for (auto& s : shields)
	{
		s->getComponent<TransformComponent>().position.x = playerPos.x - 16;
		s->getComponent<TransformComponent>().position.y = playerPos.y - 16;
	}
	//theShield.getComponent<TransformComponent>().position.x = playerPos.x - 16;
	//theShield.getComponent<TransformComponent>().position.y = playerPos.y - 16;

	manager.refresh();	
	manager.update();

	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol))
		{
			player.getComponent<TransformComponent>().position = playerPos;
		}
	}

	for (auto& p : projectiles)
	{
		for (auto& s : shields)
		{
			if (Collision::AABB(s->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
			{
				setShield(false);
				s->destroy();
				p->destroy();
			}
		}
		if (Collision::AABB(playerCol, p->getComponent<ColliderComponent>().collider)) {
			//p->destroy();
			player.destroy();
		}
		for (auto& c : colliders)
		{
			if (Collision::AABB(c->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
			{
				p->destroy();
				/*p->getComponent<TransformComponent>().velocity.x = -(p->getComponent<TransformComponent>().velocity.x); //studsande projectiles
				p->getComponent<TransformComponent>().velocity.y = -(p->getComponent<TransformComponent>().velocity.y);*/
			}
		}
	}

	/*
	camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - (Game::gameSize/2));
	camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - (Game::gameSize/2));

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;						//Kamera följer player
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;
	*/

	/*Vector2D playerVelocity = player.getComponent<TransformComponent>().velocity;
	int playerSpeed = player.getComponent<TransformComponent>().speed;

	for (auto t : tiles)
	{
		t->getComponent<TileComponent>().destRect.x += -(playerVelocity.x * playerSpeed);
		t->getComponent<TileComponent>().destRect.y += -(playerVelocity.y * playerSpeed);
	}
	for (auto cc : colliders)
	{
		Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
	}*/ //Map scroll
}

void Game::render()
{
	SDL_RenderClear(renderer);
	for (auto& t : tiles)
	{
		t->draw();
	}
	
	for (auto& p : players)
	{
		p->draw();
	}

	for (auto& p : projectiles)
	{
		p->draw();
	}

	for (auto& s : shields) {
		s->draw();
	}

	/*for (auto& c : colliders)
	{
		c->draw(); //visa collider hitbox
	}*/

	SDL_RenderPresent(renderer);
}

void Game::createProjectile(float xpos, float ypos, float xSpeed, float ySpeed, int range, int speed) {
	assets->CreateProjectile(Vector2D(xpos, ypos), Vector2D(xSpeed, ySpeed), range, speed, "projectile");
}

void Game::createShield() {
	assets->CreateShield();
}

float Game::getSpeed()
{
	int random = rng();
	if (random <= 4)
		return 0.4f;
	if (random == 5 || random == 6 || random == 7)
		return 0.1f;
	if (random == 8 || random == 9 || random == 10)
		return 0.5f;
	return 0.0f;
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

#pragma once

#include "Player.h"
#include "Enemy.h"
#include "TileMap.h"

class Game
{
private:
	sf::RenderWindow* window;
	sf::Event event;

	sf::View view;
	TileMap map;

	Player player;
	Enemy enemy;

	float viewSpeed;

	void initWindow();
	void initVariables();
	void initMap();
public:
	Game();
	~Game();

	void moveView();
	void updatePollEvents();

	void update();
	void render();

	void run();
};


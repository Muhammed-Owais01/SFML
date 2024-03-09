#pragma once

#include "Player.h"
#include "Enemy.h"

class Game
{
private:
	sf::RenderWindow* window;
	sf::Event event;

	Player player;
	Enemy enemy;

	void initWindow();
	void initVariables();
public:
	Game();
	~Game();

	void updatePollEvents();

	void update();
	void render();

	void run();
};


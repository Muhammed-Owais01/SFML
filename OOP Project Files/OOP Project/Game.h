#pragma once

#include "Player.h"

class Game
{
private:
	sf::RenderWindow* window;
	sf::Event event;

	Player player;

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


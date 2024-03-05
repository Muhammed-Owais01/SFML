#pragma once

#include "GameState.h"

class Game
{
private:
	sf::RenderWindow* window;
	sf::Event event;

	std::stack<State*> states;

	sf::Clock dtClock;
	float dt;

	void initWindow();
	void initStates();
public:
	Game();
	~Game();

	// Functions
	void endApplication();

	// Update
	void updateDT();
	void updatePollEvents();
	void update();

	// Render
	void render();

	// Core
	void run();
};


#pragma once

#include "Platform.h"

class Enemy
	: public GameEngine
{
private:
	sf::RectangleShape enemy;

	void initVariables();
public:
	Enemy();
	~Enemy();

	void updateMovement();
	void updateWindowCollision(sf::RenderWindow& window);

	void update(sf::RenderWindow& window);
	void render(sf::RenderTarget* target);
};


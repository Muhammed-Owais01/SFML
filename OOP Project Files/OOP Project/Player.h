#pragma once

#include "Platform.h"

class Player
{
private:
	sf::CircleShape circle;
	int moveSpeed;

	Platform platform;

	void initVariables(sf::RenderWindow& window);
public:
	Player();
	Player(sf::RenderWindow& window);
	~Player();

	const sf::FloatRect& getBounds() const;

	void updateWindowCollision(sf::RenderWindow& window);
	void updatePlatformCollision();
	void updateMovement();

	void update(sf::RenderWindow& window);
	void render(sf::RenderTarget *target);
};


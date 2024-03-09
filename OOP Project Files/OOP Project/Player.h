#pragma once

#include "Platform.h"

class Player
	: public GameEngine
{
private:
	sf::CircleShape circle;

	Platform platform;

	void initVariables(sf::RenderWindow& window);
public:
	Player();
	Player(sf::RenderWindow& window);
	~Player();

	const sf::FloatRect& getBounds() const;

	void updateWindowCollision(sf::RenderWindow& window);
	void updatePlatformCollision();
	void move(const float dir_x, const float dir_y);
	void updateMovement();

	void update(sf::RenderWindow& window);
	void render(sf::RenderTarget *target);
};


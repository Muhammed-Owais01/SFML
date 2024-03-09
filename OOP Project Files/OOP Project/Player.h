#pragma once

#include "Platform.h"

class Player
{
private:
	sf::CircleShape circle;
	int moveSpeed;
	sf::Vector2f velocity;
	float velocityMin, velocityMax, acceleration, gravity, drag, velocityMaxY, jump;

	Platform platform;

	void initVariables(sf::RenderWindow& window);
	void initPhysics();
public:
	Player();
	Player(sf::RenderWindow& window);
	~Player();

	const sf::FloatRect& getBounds() const;

	void updateWindowCollision(sf::RenderWindow& window);
	void updatePlatformCollision();
	void move(const float dir_x, const float dir_y);
	void updatePhysics();
	void updateMovement();

	void update(sf::RenderWindow& window);
	void render(sf::RenderTarget *target);
};


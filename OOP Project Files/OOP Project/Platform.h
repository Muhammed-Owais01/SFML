#pragma once

#include "GameEngine.h"

class Platform
{
private:
	sf::RectangleShape shape;

	void initVariables(sf::RenderWindow& window);
public:
	Platform();
	Platform(sf::RenderWindow& window);
	~Platform();

	const sf::FloatRect& getBounds() const;
	sf::Vector2f getPos();

	void setPos(int x, int y);
	void update();
	void render(sf::RenderTarget* target);
};


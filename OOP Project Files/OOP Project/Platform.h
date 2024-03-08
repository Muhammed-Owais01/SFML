#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

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


#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <tuple>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class GameEngine
{
private:
	sf::Vector2f velocity;
	float velocityMin, velocityMax, acceleration, gravity, drag, velocityMaxY, jump;
public:
	GameEngine();
	~GameEngine();

	// Getters
	sf::Vector2f& getVelocity();
	const float& getVelocityMax() const;
	const float& getAcceleration() const;
	const float& getJump() const;

	// Setters
	void changeVelocity(float x, float y);

	virtual void initPhysics();
	void updatePhysics();

};


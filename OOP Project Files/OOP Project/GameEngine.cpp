#include "GameEngine.h"

GameEngine::GameEngine()
{
	this->initPhysics();
}

GameEngine::~GameEngine()
{

}

sf::Vector2f& GameEngine::getVelocity()
{
	return this->velocity;
}

const float& GameEngine::getVelocityMax() const
{
	return this->velocityMax;
}

const float& GameEngine::getAcceleration() const
{
	return this->acceleration;
}

const float& GameEngine::getJump() const
{
	return this->jump;
}

void GameEngine::changeVelocity(float x, float y)
{
	this->velocity.x += x;
	this->velocity.y += y;
}

void GameEngine::initPhysics()
{
	this->velocityMax = 5.f;
	this->velocityMin = 1.f;
	this->acceleration = 2.f;
	this->drag = 0.95f;
	this->gravity = 5.f;
	this->velocityMaxY = 15.f;
	this->jump = 20.f;
	this->velocity.x = 0.f;
	this->velocity.y = 0.f;
}

void GameEngine::updatePhysics()
{
	this->velocity.y += gravity;
	if (std::abs(this->velocity.y) > this->velocityMaxY)
	{
		this->velocity.y = this->velocityMaxY * ((this->velocity.y < 0) ? -1.f : 1.f);
	}

	this->velocity *= this->drag;
	if (std::abs(this->velocity.x) < this->velocityMin)
	{
		this->velocity.x = 0.f;
	}
	if (std::abs(this->velocity.y) < this->velocityMin)
	{
		this->velocity.y = 0.f;
	}
}


#include "Enemy.h"

void Enemy::initVariables()
{
	this->enemy.setFillColor(sf::Color::Yellow);
	this->enemy.setSize(sf::Vector2f(10.f, 10.f));
}

Enemy::Enemy()
{
	this->initVariables();
}

Enemy::~Enemy()
{

}

void Enemy::updateMovement()
{
	this->enemy.move(this->getVelocity());
}

void Enemy::updateWindowCollision(sf::RenderWindow& window)
{
	sf::FloatRect bounds = this->enemy.getGlobalBounds();
	if (bounds.top < 0.f)
		this->enemy.setPosition(this->enemy.getPosition().x, 0.f);
	else if (bounds.top + bounds.height > window.getSize().y)
		this->enemy.setPosition(this->enemy.getPosition().x, window.getSize().y - bounds.height);

	if (bounds.left < 0.f)
		this->enemy.setPosition(0.f, this->enemy.getPosition().y);
	else if (bounds.left + bounds.width > window.getSize().x)
		this->enemy.setPosition(window.getSize().x - bounds.width, this->enemy.getPosition().y);
}

void Enemy::update(sf::RenderWindow& window)
{
	this->updateMovement();
	this->updatePhysics();
	this->updateWindowCollision(window);
}

void Enemy::render(sf::RenderTarget* target)
{
	target->draw(this->enemy);
}

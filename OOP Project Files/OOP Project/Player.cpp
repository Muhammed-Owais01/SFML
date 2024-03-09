#include "Player.h"

void Player::initVariables(sf::RenderWindow& window)
{
	this->circle.setRadius(30.f);
	this->circle.setFillColor(sf::Color::White);
	
	this->platform = Platform(window);
}

Player::Player()
{

}

Player::Player(sf::RenderWindow& window)
{
	this->initVariables(window);
}

Player::~Player()
{

}

const sf::FloatRect& Player::getBounds() const
{
	return this->circle.getGlobalBounds();
}

void Player::updateWindowCollision(sf::RenderWindow& window)
{
	sf::FloatRect bounds = this->circle.getGlobalBounds();
	if (bounds.top < 0.f)
		this->circle.setPosition(this->circle.getPosition().x, 0.f);
	else if (bounds.top + bounds.height > window.getSize().y)
		this->circle.setPosition(this->circle.getPosition().x, window.getSize().y - bounds.height);

	if (bounds.left < 0.f)
		this->circle.setPosition(0.f, this->circle.getPosition().y);
	else if (bounds.left + bounds.width > window.getSize().x)
		this->circle.setPosition(window.getSize().x - bounds.width, this->circle.getPosition().y);
}

void Player::updatePlatformCollision()
{
	sf::Vector2f prevPlayerPos = this->circle.getPosition();
	sf::FloatRect newPlayerPos = this->circle.getGlobalBounds();

	float playerLeft = newPlayerPos.left;
	float playerRight = newPlayerPos.left + newPlayerPos.width;
	float playerTop = newPlayerPos.top;
	float playerBottom = newPlayerPos.top + newPlayerPos.height;

	float platformLeft = this->platform.getBounds().left;
	float platformRight = this->platform.getBounds().left + this->platform.getBounds().width;
	float platformTop = this->platform.getBounds().top;
	float platformBottom = this->platform.getBounds().top + this->platform.getBounds().height;


	if (newPlayerPos.intersects(this->platform.getBounds()))
	{
		// Box Top
		if ((playerBottom >= platformTop && playerBottom < platformBottom)
			&& (playerRight >= platformLeft + std::abs(this->getVelocity().x * 2) && playerLeft < platformRight - std::abs(this->getVelocity().x * 2)))
			this->circle.setPosition(this->circle.getPosition().x, this->platform.getBounds().top - newPlayerPos.height);

		// Box Left
		else if ((playerRight > platformLeft && playerRight < platformRight)
			&& (playerBottom >= platformTop + std::abs(this->getVelocity().y * 2) && playerTop <= platformBottom - std::abs(this->getVelocity().y * 2)))
			this->circle.setPosition(this->platform.getBounds().left - newPlayerPos.width, this->circle.getPosition().y);

		// Box Bottom
		else if ((playerTop < platformBottom && playerBottom > platformTop)
			&& (playerRight >= platformLeft + std::abs(this->getVelocity().x * 2) && playerLeft <= platformRight - std::abs(this->getVelocity().x * 2)))
			this->circle.setPosition(this->circle.getPosition().x, this->platform.getBounds().top + this->platform.getBounds().height);

		// Box Right
		else if ((playerLeft < platformRight && playerRight > platformLeft + std::abs(this->getVelocity().x * 2))
			&& (playerBottom >= platformTop && playerTop <= platformBottom))
			this->circle.setPosition(this->platform.getBounds().left + this->platform.getBounds().width, this->circle.getPosition().y);
	}
}

void Player::move(const float dir_x, const float dir_y)
{
	this->changeVelocity(dir_x * this->getAcceleration(), 0.f);

	if (std::abs(this->getVelocity().x) > this->getVelocityMax())
	{
		this->getVelocity().x = this->getVelocityMax() * ((this->getVelocity().x < 0) ? -1.f : 1.f);
	}

	this->getVelocity().y = dir_y * this->getJump();
}

void Player::updateMovement()
{
	// JUMP
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		this->move(0.f, -1.f);
	}
	// Move Backwards
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->move(-1.f, 0.f);
	}
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->circle.move(0.f, this->moveSpeed);
	}*/
	// Move Forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->move(1.f, 0.f);
	}
	// Jump Forward
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->move(1.f, -1.f);
	}
	this->circle.move(this->getVelocity());
}


void Player::update(sf::RenderWindow& window)
{
	this->updatePhysics();
	this->updateMovement();
	this->updateWindowCollision(window);
	this->updatePlatformCollision();
}

void Player::render(sf::RenderTarget* target)
{
	target->draw(this->circle);
	this->platform.render(target);
}

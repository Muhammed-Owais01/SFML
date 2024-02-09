#include "stdafx.h"
#include "Player.h"

void Player::initVariables()
{
	this->animState = IDLE;
}

void Player::initTexture()
{
	if (!this->textureSheet.loadFromFile("Assets/player_sheet.png"))
		std::cout << "ERROR::PLAYER::Could not load player_sheet.png" << std::endl;
}

void Player::initSprite()
{
	this->sprite.setTexture(this->textureSheet);
	this->currentFrame = sf::IntRect(0, 0, 99, 100); // TO GET THE SIZE, OPEN IMAGE PROPERTIES, DIVIDE WIDTH BY THE MAX SPRITE HORIZONTALLY, HEIGHT DIVIDE BY VERTICAL
	this->sprite.setTextureRect(this->currentFrame);
	this->sprite.setScale(1.1f, 1.1f);
}

void Player::initAnimations()
{
	this->animation_timer.restart();
	this->animationSwitch = true;
}

void Player::initPhysics()
{
	this->velocityMax = 5.f;
	this->velocityMin = 1.f;
	this->acceleration = 3.f;
	this->drag = 0.93f;
	this->gravity = 4.f;
	this->velocityMaxY = 15.f;
}

Player::Player()
{
	this->initVariables();
	this->initTexture();
	this->initSprite();
	this->initAnimations();
	this->initPhysics();
}

Player::~Player()
{

}

const bool& Player::getAnimSwitch()
{
	bool anim_switch = this->animationSwitch;

	if (this->animationSwitch)
		this->animationSwitch = false;

	return anim_switch;
}

const sf::Vector2f Player::getPosition() const
{
	return this->sprite.getPosition();
}

const sf::FloatRect& Player::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

void Player::setPosition(const float x, const float y)
{
	this->sprite.setPosition(x, y);
}

void Player::resetVelocityY()
{
	this->velocity.y = 0.f;
}

void Player::resetAnimationTimer()
{
	this->animation_timer.restart();
	this->animationSwitch = true;
}

void Player::move(const float dir_x, const float dir_y)
{
	// Acceleration
	this->velocity.x += dir_x * this->acceleration;

	// Limit Velocity
	if (std::abs(this->velocity.x) > this->velocityMax)
	{
		this->velocity.x = this->velocityMax * ((this->velocity.x < 0.f) ? -1.f : 1.f);
	}
}

void Player::updatePhysics()
{
	// Gravity
	this->velocity.y += 1.0 * this->gravity;
	if (std::abs(this->velocity.y) > this->velocityMaxY)
	{
		this->velocity.y = this->velocityMaxY * ((this->velocity.y < 0.f) ? -1.f : 1.f);
	}

	// Deceleration
	this->velocity *= this->drag;

	// Limit deceleration
	if (abs(this->velocity.x) < this->velocityMin)
		this->velocity.x = 0.f;
	if (abs(this->velocity.y) < this->velocityMin)
		this->velocity.y = 0.f;

	this->sprite.move(this->velocity);
}

void Player::updateMovement()
{
	this->animState = IDLE;
	// Left 
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
	{
		this->move(-1.f, 0.f);
		this->animState = MOVING_LEFT;
	}
	// Right 
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->move(1.f, 0.f);
		this->animState = MOVING_RIGHT;
	}

	//// Top 
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	//{
	//	this->sprite.move(0.f, -1.f);
	//}
	//// Bottom 
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	//{
	//	this->sprite.move(0.f, 1.f);
	//}
}

void Player::updateAnimations()
{
	// IDLE Animations
	if (this->animState == IDLE)
	{
		if (this->animation_timer.getElapsedTime().asSeconds() >= 0.4f || this->getAnimSwitch())
		{
			this->currentFrame.top = 0.f;
			this->currentFrame.left += 99;
			if (this->currentFrame.left >= 495.f)
				this->currentFrame.left = 0;

			this->animation_timer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}
	}
	else if (this->animState == MOVING_LEFT)
	{
		if (this->animation_timer.getElapsedTime().asSeconds() >= 0.14f || this->getAnimSwitch())
		{
			this->currentFrame.top = 200;
			this->currentFrame.left += 99;
			if (this->currentFrame.left >= 594.f)
				this->currentFrame.left = 0;

			this->animation_timer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}

		this->sprite.setScale(1.1f, 1.1f);
		this->sprite.setOrigin(0.f, 0.f);
	}
	else if (this->animState == MOVING_RIGHT)
	{
		if (this->animation_timer.getElapsedTime().asSeconds() >= 0.14f || this->getAnimSwitch())
		{
			this->currentFrame.top = 200;
			this->currentFrame.left += 99;
			if (this->currentFrame.left >= 594.f)
				this->currentFrame.left = 0;

			this->animation_timer.restart();
			this->sprite.setTextureRect(this->currentFrame);
		}

		this->sprite.setScale(-1.1f, 1.1f);
		this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 1.1f, 0.f);
	}
	else
		this->animation_timer.restart();
	
}

void Player::update()
{
	this->updateMovement();
	this->updateAnimations();
	this->updatePhysics();
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);

	sf::CircleShape circ;
	circ.setFillColor(sf::Color::Red);
	circ.setRadius(2.f);
	circ.setPosition(this->sprite.getPosition());

	target.draw(circ);
}

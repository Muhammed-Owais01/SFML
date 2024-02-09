#include "stdafx.h"
#include "Game.h"

void Game::initWindow()
{
	this->window.create(sf::VideoMode(800, 600), "Platformer", sf::Style::Close | sf::Style::Titlebar);
	this->window.setFramerateLimit(60);
}

void Game::initPlayer()
{
	this->player = new Player();
}

Game::Game()
{
	this->initWindow();
	this->initPlayer();
}

Game::~Game()
{
	delete this->player;
}

void Game::updatePlayer()
{
	this->player->update();
}

void Game::updateCollision()
{
	// Collision Bottom Screen
	if (this->player->getPosition().y + this->player->getBounds().height > this->window.getSize().y)
	{
		this->player->resetVelocityY();
		this->player->setPosition(
			this->player->getPosition().x,
			this->window.getSize().y - this->player->getBounds().height
			);
	}
}

void Game::update()
{
	while (this->window.pollEvent(this->event))
	{
		if (this->event.type == sf::Event::Closed)
			this->window.close();
		if (this->event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			this->window.close();

		if (this->event.type == sf::Event::KeyReleased && 
			(	
				this->event.key.code == sf::Keyboard::A ||
				this->event.key.code == sf::Keyboard::D ||
				this->event.key.code == sf::Keyboard::W ||
				this->event.key.code == sf::Keyboard::S 
				)
			)
			this->player->resetAnimationTimer();
	}

	this->updatePlayer();

	this->updateCollision();
}

void Game::renderPlayer()
{
	this->player->render(this->window);
}

void Game::render()
{
	this->window.clear();

	// Draw
	this->renderPlayer();

	this->window.display();
}

const sf::RenderWindow& Game::getWindow() const
{
	// TODO: insert return statement here
	return this->window;
}

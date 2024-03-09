#include "Game.h"

void Game::initWindow()
{
	std::ifstream ifs("Config/window.ini");

	std::string title = "None";
	sf::VideoMode windowBounds(800, 600);
	unsigned frameLimit = 120;
	bool vertical_sync_enabled = false;

	if (ifs.is_open())
	{
		getline(ifs, title);
		ifs >> windowBounds.width, windowBounds.height;
		ifs >> frameLimit;
		ifs >> vertical_sync_enabled;
	}
	this->window = new sf::RenderWindow(windowBounds, title);
	this->window->setFramerateLimit(frameLimit);
	this->window->setVerticalSyncEnabled(vertical_sync_enabled);
}

void Game::initVariables()
{
	this->player = Player(*this->window);
}

Game::Game()
{
	this->initWindow();
	this->initVariables();
}

Game::~Game()
{

}

void Game::updatePollEvents()
{
	while (this->window->pollEvent(this->event))
	{
		if (this->event.type == sf::Event::Closed)
			this->window->close();
	}
}

void Game::update()
{
	this->updatePollEvents();

	this->player.update(*this->window);

	this->enemy.update(*this->window);
}

void Game::render()
{
	this->window->clear();

	this->player.render(this->window);

	this->enemy.render(this->window);

	this->window->display();
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->update();
		this->render();
	}
}

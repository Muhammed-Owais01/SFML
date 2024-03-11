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

	this->view.setSize(windowBounds.width, windowBounds.height);
	this->view.setCenter(sf::Vector2f(this->window->getSize().x / 2.f, this->window->getSize().y / 2.f));
}

void Game::initVariables()
{
	this->player = Player(*this->window);
	this->viewSpeed = 10.f;
}

void Game::initMap()
{
	const int level[] =
	{
		17
	};
	int num1, num2, num3;
	std::ifstream inputFile("Map/MapData.txt");
	std::vector<int> Test;

	std::vector<std::tuple<int, int, int>> tiles;

	if (inputFile.is_open())
	{
		inputFile >> num1 >> num2 >> num3;
		tiles.push_back(std::make_tuple(num1, num2, num3));
	}
	inputFile.close();

	if (!map.load("Textures/test.png", sf::Vector2u(30, 30), Test, 1, 1))
		std::cout << "COULD NOT LOAD tileset.png";
}

Game::Game()
{
	this->initWindow();
	this->initVariables();
	this->initMap();
}

Game::~Game()
{

}

void Game::moveView()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->view.move(sf::Vector2f(0.f, -viewSpeed));
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->view.move(sf::Vector2f(0.f, viewSpeed));
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->view.move(sf::Vector2f(-viewSpeed, 0.f));
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->view.move(sf::Vector2f(viewSpeed, 0.f));
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

	//this->moveView();
	this->player.update(*this->window);

	this->enemy.update(*this->window);
}

void Game::render()
{
	this->window->clear();

	// Render in View
	this->window->setView(this->view);

	this->window->draw(this->map);

	this->player.render(this->window);

	this->enemy.render(this->window);

	// Render UI
	this->window->setView(this->window->getDefaultView());

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

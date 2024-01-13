#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <cstdlib>

class Bullet
{
public:
	sf::Sprite bullet;

	Bullet(sf::Texture* texture, sf::Vector2f pos)
	{
		this->bullet.setTexture(*texture);
		this->bullet.setScale(sf::Vector2f(0.5f, 0.5f));
		this->bullet.setPosition(pos);
	}
};

class Player
{
public:
	sf::Sprite shape;
	sf::Texture* texture;
	int hp;
	int maxHp;
	std::vector<Bullet> bullets;
	Player(sf::Texture *texture, sf::Vector2u window)
		: maxHp(15)
	{
		this->texture = texture;
		this->shape.setTexture(*texture);
		this->hp = maxHp;

		this->shape.setScale(sf::Vector2f(0.7f, 0.7f));
		this->shape.setPosition(sf::Vector2f(window.x / 2 - this->shape.getGlobalBounds().width, window.y - this->shape.getGlobalBounds().height - 25));
	}
};

class Enemy
{
public:
	sf::Sprite enemy;
	int points;
	bool shoots;
	int enemyShootTimer;
	int hp;
	std::vector<Bullet> bullets1;
	std::vector<Bullet> bullets2;
	Enemy(sf::Texture* texture, sf::Vector2u window, float scale, int points, bool shoots, std::vector<Enemy>& enemies)
		: enemyShootTimer(0)
	{
		this->enemy.setTexture(*texture);
		this->enemy.setScale(sf::Vector2f(scale, scale));
		this->points = points;
		this->shoots = shoots;
		this->hp = points;

		do
		{
			this->enemy.setPosition(sf::Vector2f(rand() % (int)(window.x - enemy.getGlobalBounds().width), 0 - this->enemy.getGlobalBounds().height));
		} while (checkOverlap(window, enemies));
	}
private:
	bool checkOverlap(sf::Vector2u window, std::vector<Enemy>& enemies)
	{
		for (auto& existingEnemy : enemies)
		{
			if (this->enemy.getGlobalBounds().intersects(existingEnemy.enemy.getGlobalBounds()))
				return true;
		}
		return false;
	}
};


int main()
{
	srand(time(NULL));

	sf::RenderWindow window(sf::VideoMode(800, 600), "Shooter Game");
	window.setFramerateLimit(60);

	// Enemies ================================================================
	sf::Texture rock1Tex;
	sf::Texture rock2Tex;
	sf::Texture rock3Tex;
	sf::Texture rock4Tex;
	sf::Texture rock5Tex;
	sf::Texture enemy_1;
	sf::Texture enemy_2;

	if (!rock1Tex.loadFromFile("space_shooter_assets/rock1.png"))
		throw "Could not load rock1.png";

	if (!rock2Tex.loadFromFile("space_shooter_assets/rock2.png"))
		throw "Could not load rock2.png";

	if (!rock3Tex.loadFromFile("space_shooter_assets/rock3.png"))
		throw "Could not load rock3.png";

	if (!rock4Tex.loadFromFile("space_shooter_assets/rock4.png"))
		throw "Could not load rock4.png";

	if (!rock5Tex.loadFromFile("space_shooter_assets/rock5.png"))
		throw "Could not load rock5.png";

	if (!enemy_1.loadFromFile("space_shooter_assets/enemy_1.png"))
		throw "Could not load enemy_1.png";

	if (!enemy_2.loadFromFile("space_shooter_assets/enemy_2.png"))
		throw "Could not load enemy_2.png";
//	sf::RectangleShape enemy;
//	enemy.setFillColor(sf::Color::Magenta);
	int spawnTime = 0;
	int score = 0;
	int rock = 0;
	bool moveUp = false;

	std::vector<Enemy> enemies;

	// Player ===============================================================================
	sf::Texture playerTex;
	if (!playerTex.loadFromFile("space_shooter_assets/player_ship.png"))
		throw "Could not load player png";
	Player player(&playerTex, window.getSize());

	// UI
	sf::Font font;
	if (!font.loadFromFile("Fonts/big_noodle_titling.ttf"))
		throw "could not load font";

	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setPosition(0.f, 0.f);

	sf::Text playerHealthText;
	playerHealthText.setFont(font);
	playerHealthText.setCharacterSize(15);
	playerHealthText.setFillColor(sf::Color::White);

	int level = 5;
	sf::Clock levelTimer;
	int maxTime = 30;

	// Projectile =============================================================
	sf::Texture projectileTex;
	sf::Texture enemyProjectileTex;

	if (!projectileTex.loadFromFile("space_shooter_assets/player_laser.png"))
		throw "could not load projectile png";

	if (!enemyProjectileTex.loadFromFile("space_shooter_assets/enemy_laser.png"))
		throw "could not load projectile png";

	/*sf::CircleShape projectile;
	projectile.setFillColor(sf::Color::Red);
	projectile.setRadius(5.f);*/

	sf::Vector2f playerCenter;
	sf::Vector2f enemyCenter;
	int shootTimer = 0;

	// Game Over ============================
	bool gameOver = false;
	sf::Texture over;
	sf::Sprite END;
	if (!over.loadFromFile("space_shooter_assets/gameover.png"))
		throw "could not load gameover.png";
	END.setTexture(over);
	END.setPosition(window.getSize().x / 2 - END.getGlobalBounds().width + 160.f, window.getSize().y / 2 - END.getGlobalBounds().height + 160.f);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();
		}
		if (gameOver == false)
		{
			// Update

			// Level
			if (levelTimer.getElapsedTime().asSeconds() >= maxTime)
			{
				maxTime += 30;
				levelTimer.restart();
				level++;
			}

			playerCenter = sf::Vector2f(player.shape.getPosition().x + player.shape.getGlobalBounds().width / 2 - 3.5f, player.shape.getPosition().y + player.shape.getGlobalBounds().height / 2);

			// Enemies
			if (spawnTime <= 60)
				spawnTime++;
			
			// Rocks
			if (spawnTime >= 20 && level <= 3)
			{
				rock = rand() % 5 + 1;
				if (rock == 1)
					enemies.push_back(Enemy(&rock1Tex, window.getSize(), 1.0, 1, false, enemies));
				else if (rock == 2)
					enemies.push_back(Enemy(&rock2Tex, window.getSize(), 1.1, 2, false, enemies));
				else if (rock == 3 && level >= 2)
					enemies.push_back(Enemy(&rock3Tex, window.getSize(), 1.15, 3, false, enemies));
				else if (rock == 4 && level == 3)
					enemies.push_back(Enemy(&rock4Tex, window.getSize(), 1.17, 4, false, enemies));
				else if (rock == 5 && level == 3)
					enemies.push_back(Enemy(&rock5Tex, window.getSize(), 1.2, 5, false, enemies));
				spawnTime = 0;
			}

			// Enemy Ships
			if (spawnTime >= 60 && level > 3)
			{
				rock = (rand() % (level - 3)) + 4;
				if (rock == 4)
					enemies.push_back(Enemy(&enemy_1, window.getSize(), 0.4, 2, true, enemies));
				else if (rock == 5 && level >= 5)
					enemies.push_back(Enemy(&enemy_2, window.getSize(), 0.6, 4, true, enemies));
				spawnTime = 0;
			}

			for (int i = 0; i < enemies.size(); i++)
			{
				if (enemies[i].shoots == false)
				{
					enemies[i].enemy.move(0, 5.f);
					if (enemies[i].enemy.getPosition().y > window.getSize().y)
						enemies.erase(enemies.begin() + i);
				}
				else
				{
					enemies[i].enemy.move(0, 2.f);
					if (enemies[i].enemy.getTexture() == &enemy_1)
					{
						enemyCenter = sf::Vector2f(enemies[i].enemy.getPosition().x + enemies[i].enemy.getGlobalBounds().width / 2 - 6.f, enemies[i].enemy.getPosition().y + enemies[i].enemy.getGlobalBounds().height / 2);
						if (enemies[i].enemyShootTimer <= 40)
							enemies[i].enemyShootTimer++;
						if (enemies[i].enemyShootTimer >= 40)
						{
							enemies[i].bullets1.push_back(Bullet(&enemyProjectileTex, enemyCenter));
							enemies[i].enemyShootTimer = 0;
						}
					}
					else if (enemies[i].enemy.getTexture() == &enemy_2)
					{
						enemyCenter = sf::Vector2f(enemies[i].enemy.getPosition().x + enemies[i].enemy.getGlobalBounds().width / 2 + 6.f, enemies[i].enemy.getPosition().y + enemies[i].enemy.getGlobalBounds().height / 2);
						if (enemies[i].enemyShootTimer <= 25)
							enemies[i].enemyShootTimer++;
						if (enemies[i].enemyShootTimer >= 25)
						{
							enemies[i].bullets1.push_back(Bullet(&enemyProjectileTex, enemyCenter));
							enemyCenter = sf::Vector2f(enemies[i].enemy.getPosition().x + enemies[i].enemy.getGlobalBounds().width / 2 - 18.f, enemies[i].enemy.getPosition().y + enemies[i].enemy.getGlobalBounds().height / 2);
							enemies[i].bullets2.push_back(Bullet(&enemyProjectileTex, enemyCenter));
							enemies[i].enemyShootTimer = 0;
						}
					}
					for (size_t j = 0; j < enemies[i].bullets1.size(); j++)
					{
						enemies[i].bullets1[j].bullet.move(0, 5.f);
						if (enemies[i].bullets1[j].bullet.getPosition().y > window.getSize().y)
							enemies[i].bullets1.erase(enemies[i].bullets1.begin() + j);
						else
						{
							if (enemies[i].bullets1[j].bullet.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
							{
								player.hp--;
								enemies[i].bullets1.erase(enemies[i].bullets1.begin() + j);
								break;
							}
						}
					}
					if (enemies[i].enemy.getTexture() == &enemy_2)
					{
						for (size_t j = 0; j < enemies[i].bullets2.size(); j++)
						{
							enemies[i].bullets2[j].bullet.move(0, 5.f);
							if (enemies[i].bullets2[j].bullet.getPosition().y > window.getSize().y)
								enemies[i].bullets2.erase(enemies[i].bullets2.begin() + j);
							else if (enemies[i].bullets2[j].bullet.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
							{
								player.hp--;
								enemies[i].bullets2.erase(enemies[i].bullets2.begin() + j);
								break;
							}
						}
					}
				}
			}

			// Player

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && player.shape.getPosition().y > 0)
			{
				player.shape.move(sf::Vector2f(0.f, -8.f));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && player.shape.getPosition().y < window.getSize().y - player.shape.getGlobalBounds().height)
			{
				player.shape.move(sf::Vector2f(0.f, 8.f));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && player.shape.getPosition().x > 0)
			{
				player.shape.move(sf::Vector2f(-8.f, 0.f));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && player.shape.getPosition().x < window.getSize().x - player.shape.getGlobalBounds().width)
			{
				player.shape.move(sf::Vector2f(8.f, 0));
			}
			playerHealthText.setPosition(sf::Vector2f(player.shape.getPosition().x - 5.f, player.shape.getPosition().y + player.shape.getGlobalBounds().height / 2 - 23.f));

			// This was how to make sure with mouse i dont move out of screen
			/*if (player.getPosition().x <= 0)
				player.setPosition(sf::Vector2f(0.f, player.getPosition().y));

			if (player.getPosition().x >= window.getSize().x - player.getGlobalBounds().width)
				player.setPosition(sf::Vector2f(window.getSize().x - player.getGlobalBounds().width, player.getPosition().y));*/

			// Projectile
			if (shootTimer <= 7)
				shootTimer++;

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootTimer >= 7)
			{
				player.bullets.push_back(Bullet(&projectileTex, playerCenter));
				shootTimer = 0;
			}
			// Collision and OutOfBounds
			for (size_t i = 0; i < player.bullets.size(); i++)
			{
				player.bullets[i].bullet.move(sf::Vector2f(0, -10.f));
				if (player.bullets[i].bullet.getPosition().y < 0)
				{
					player.bullets.erase(player.bullets.begin() + i);
				}
				else
				{
					for (size_t j = 0; j < enemies.size(); j++)
					{
						// Bullet on rock collision
						if (player.bullets[i].bullet.getGlobalBounds().intersects(enemies[j].enemy.getGlobalBounds()) && enemies[j].shoots == false)
						{
							score += enemies[j].points;
							player.bullets.erase(player.bullets.begin() + i);
							enemies.erase(enemies.begin() + j);
							break;
						}
						// Bullet on ship collision
						else if (player.bullets[i].bullet.getGlobalBounds().intersects(enemies[j].enemy.getGlobalBounds()) && enemies[j].shoots == true)
						{
							score += enemies[j].points;
							player.bullets.erase(player.bullets.begin() + i);
							enemies[j].hp--;
							if (enemies[j].hp <= 0)
								enemies.erase(enemies.begin() + j);
							break;
						}
					}
				}
			}

			// Enemy collision
			for (size_t i = 0; i < enemies.size(); i++)
			{
				if (player.shape.getGlobalBounds().intersects(enemies[i].enemy.getGlobalBounds()))
				{
					enemies.erase(enemies.begin() + i);
					player.hp--;
					break;
				}
			}
			if (player.hp <= 0)
				gameOver = true;

			// UI
			// Player Score
			std::string scoreString = "Score: " + std::to_string(score);
			text.setString(scoreString);
			text.setPosition(0.f, 0.f);

			// Player Health
			playerHealthText.setString(std::to_string(player.hp) + "\\" + std::to_string(player.maxHp));
		}
		// Clear
		window.clear(sf::Color::Black);
		// Draw
		window.draw(player.shape);
		for (int i = 0; i < enemies.size(); i++)
		{
			window.draw(enemies[i].enemy);
			for (int j = 0; j < enemies[i].bullets1.size(); j++)
			{
				window.draw(enemies[i].bullets1[j].bullet);
			}
			if (enemies[i].enemy.getTexture() == &enemy_2)
			{
				for (size_t j = 0; j < enemies[i].bullets2.size(); j++)
				{
					window.draw(enemies[i].bullets2[j].bullet);
				}
			}
		}

		for (int i = 0; i < player.bullets.size(); i++)
		{
			window.draw(player.bullets[i].bullet);
		}

		window.draw(text);
		text.setString("Timer: " + std::to_string(static_cast<int>(levelTimer.getElapsedTime().asSeconds())));
		text.setPosition(window.getSize().x - text.getGlobalBounds().width, 0.f);
		window.draw(text);
		text.setString("Level " + std::to_string(level));
		text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width - 10, window.getSize().y - text.getGlobalBounds().height * 2);
		window.draw(text);
		window.draw(playerHealthText);
		if (gameOver == true)
		{
			window.draw(END);
		}
		// Display
		window.display();
	}
}
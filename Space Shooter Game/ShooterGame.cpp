#include <iostream>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <cstdlib>

// Creates a bullet class
class Bullet
{
public:
	sf::Sprite bullet;

	Bullet(sf::Texture* texture, sf::Vector2f pos)
	{
		this->bullet.setTexture(*texture);
		this->bullet.setScale(sf::Vector2f(0.5f, 0.5f));
		// position of the bullet (center of player or enemy mostly)
		this->bullet.setPosition(pos);
	}
};

// Creates a player class
class Player
{
public:
	sf::Sprite shape;
	sf::Texture* texture;
	int hp;
	int maxHp;
	// Stores the power up number
	int powerUp;
	// Player has bullets
	std::vector<std::vector<Bullet>> bullets;
	// Vector to be used when player gets rockets
	std::vector<Bullet> rockets;
	Player(sf::Texture *texture, sf::Vector2u window)
		: maxHp(20), powerUp(-1)
	{
		this->texture = texture;
		this->shape.setTexture(*texture);
		this->hp = maxHp;

		this->shape.setScale(sf::Vector2f(0.7f, 0.7f));
		this->shape.setPosition(sf::Vector2f(window.x / 2 - this->shape.getGlobalBounds().width, window.y - this->shape.getGlobalBounds().height - 25));
		// To define the 2d vector size
		bullets.resize(2);
	}
};

// Creates enemy class
class Enemy
{
public:
	sf::Sprite enemy;
	int points;
	sf::RectangleShape enemyHealthBar;
	// to differentiate between rocks and ships, true in ships
	bool shoots;
	// calculates the time for shooting
	int enemyShootTimer;
	int hp;
	// Multiple number of bullets, thus 2d vector
	std::vector<std::vector<Bullet>> bullets;

	Enemy(sf::Texture* texture, int length, sf::Vector2u window, float scale, int points, bool shoots, std::vector<Enemy>& enemies)
		: enemyShootTimer(0)
	{
		this->enemy.setTexture(*texture);
		this->enemy.setScale(sf::Vector2f(scale, scale));
		this->points = points;
		this->shoots = shoots;
		this->hp = points;
		this->enemyHealthBar.setFillColor(sf::Color::Green);
		// Health bar set on the size of hp
		this->enemyHealthBar.setSize(sf::Vector2f((float)this->hp * 15.f, 5.f));
		// 2d Vector Size set on length which is basically the number of bullets shot by an enemy
		bullets.resize(length);
		
		// this ensures that sprites dont spawn overlapping each other
		// checkOverlap executes the moment a sprite is spawned, thus changing its position outside of the screen to a place where no collision happens
		do
		{
			this->enemy.setPosition(sf::Vector2f(rand() % (int)(window.x - enemy.getGlobalBounds().width), 0 - this->enemy.getGlobalBounds().height));
		} while (checkOverlap(window, enemies));
	}
	// for firing the bullets
	void bulletsFire(int gunNum, sf::Vector2f enemyCenter, float movement, sf::Vector2u window, Player *player)
	{
		// Shoots all the bullets in a gun, the gun number is given by gunNum
		for (size_t i = 0; i < bullets[gunNum].size(); i++)
		{
			bullets[gunNum][i].bullet.move(movement, 5.f);
			if (bullets[gunNum][i].bullet.getPosition().y > window.y)
				bullets[gunNum].erase(bullets[gunNum].begin() + i);
			else
			{
				if (bullets[gunNum][i].bullet.getGlobalBounds().intersects((*player).shape.getGlobalBounds()))
				{
					// if the player acquires the sheild then dont let player take any damage
					if (player->powerUp != 1)
						(*player).hp--;
					bullets[gunNum].erase(bullets[gunNum].begin() + i);
				}
			}
		}
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
	sf::Texture rock1Tex, rock2Tex, rock3Tex, rock4Tex, rock5Tex;
	sf::Texture enemy_1, enemy_2, enemy_3;

	// Loading all the rocks and enemy textures
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

	if (!enemy_3.loadFromFile("space_shooter_assets/enemy_3.png"))
		throw "Could not load enemy_3.png";

	//BOSS =====================================================================================
	sf::Texture bossTex;

	// Loading boss texture
	if (!bossTex.loadFromFile("space_shooter_assets/boss.png"))
		throw "Could not load boss.png";

	// Created a boss vector mainly because it has a class enemy whoose constructor needs a vector
	std::vector<Enemy> boss;
	// Boss 8 gun, size 1.f, points 100, shoots is true
	boss.push_back(Enemy(&bossTex, 8, window.getSize(), 1.f, 100, true, boss));
	// Sets its position above the screen
	boss[0].enemy.setPosition(window.getSize().x / 2, 0 - boss[0].enemy.getGlobalBounds().height);
	boss[0].hp = 100;
	// Setting health bar of the boss
	boss[0].enemyHealthBar.setSize(sf::Vector2f((float)boss[0].hp * 5.f, 40.f));
	boss[0].enemyHealthBar.setPosition(sf::Vector2f(window.getSize().x - boss[0].enemyHealthBar.getGlobalBounds().width * 1.5f + 105.f, 5.f));
	boss[0].enemyHealthBar.setFillColor(sf::Color::Red);
	// Frequency for shooting lower guns
	int bossShotTimerLower = 0;
	// Frequency for shooting upper guns
	int bossShotTimerUpper = 0;
	// Stop moving the boss once it reaches its location
	bool bossStop = false;
	// Left or Right movement of boss
	int dir = 0;
	// The boss movement x value, so that its guns bullets move in that direction too
	float bossMove = 0;
	// Determines which if loop to go in, whether the loop of lower guns or upper guns
	int LowerOrUpperGun = 0;
	
	// The amount of time the lower or upper guns are fired
	sf::Clock LoF;
	// Time when to fire the beams
	sf::Clock BeamTimer;
	// Time the beams stay being fired
	sf::Clock time;

	// ENEMY =====================================================================================
	// Frequency of spawning the enemies
	int spawnTime = 0;
	// Overall score of the player
	int score = 0;
	// Which enemy is to be spawned
	int rock = 0;

	// Stores all the enemies
	std::vector<Enemy> enemies;

	// Player ===============================================================================
	sf::Texture playerTex;
	if (!playerTex.loadFromFile("space_shooter_assets/player_ship.png"))
		throw "Could not load player png";
	Player player(&playerTex, window.getSize());

	// POWER UPS ================================================================================

	// The timer for spawning powerups
	sf::Clock powerUpTimer;
	// If powerup generated do not generate anymore
	bool generatedPowerUp = false;
	// The powerup number
	int randomPowerUp = -1;
	// For turning sheild on or off
	bool sheildOnOff = false;
	// For determining if rocket has been shot
	bool rocketShot = false;
	// Sprites and textures of power ups loaded
	sf::Sprite doubleGun, sheild, sheildGlow, health, rocket, rocketIcon;
	std::vector<sf::Sprite> powerUps;
	sf::Texture doubleGunTex, sheildTex, sheildGlowTex, healthTex, rocketTex, rocketIconTex;
	if (!doubleGunTex.loadFromFile("space_shooter_assets/double_gun.png"))
		throw "Could not load double_gun.png";
	doubleGun.setTexture(doubleGunTex);
	doubleGun.setScale(sf::Vector2f(0.7f, 0.7f));
	powerUps.push_back(sf::Sprite(doubleGun));
	if (!sheildTex.loadFromFile("space_shooter_assets/shield.png"))
		throw "Could not load shield.png";
	sheild.setTexture(sheildTex);
	sheild.setScale(sf::Vector2f(0.05f, 0.05f));
	powerUps.push_back(sf::Sprite(sheild));
	if (!sheildGlowTex.loadFromFile("space_shooter_assets/sheildglow.png"))
		throw "Could not load sheildglow.png";
	sheildGlow.setTexture(sheildGlowTex);
	sheildGlow.setScale(sf::Vector2f(0.7f, 0.7f));
	if (!healthTex.loadFromFile("space_shooter_assets/health.png"))
		throw "Could not load health.png";
	health.setTexture(healthTex);
	health.setScale(sf::Vector2f(0.06f, 0.06f));
	powerUps.push_back(sf::Sprite(health));
	if (!rocketTex.loadFromFile("space_shooter_assets/rocket.png"))
		throw "Could not load rocket.png";
	rocket.setTexture(rocketTex);
	powerUps.push_back(rocket);
	if (!rocketIconTex.loadFromFile("space_shooter_assets/rocketIcon.png"))
		throw "Could not load rocketIcon.png";
	rocketIcon.setTexture(rocketIconTex);
	rocketIcon.setPosition(window.getSize().x - rocketIcon.getGlobalBounds().width, window.getSize().y - rocketIcon.getGlobalBounds().height);

	// Setting the random x position above the screen for powerups
	for (size_t i = 0; i < powerUps.size(); i++)
		powerUps[i].setPosition(sf::Vector2f(rand() % (int)(window.getSize().x - powerUps[i].getGlobalBounds().width), 0 - powerUps[i].getGlobalBounds().height));

	// UI =============================================================================================
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

	sf::Text rocketCount;
	rocketCount.setFont(font);
	rocketCount.setCharacterSize(24);
	rocketCount.setFillColor(sf::Color::White);
	rocketCount.setPosition(rocketIcon.getPosition());

	int level = 1;
	sf::Clock levelTimer;
	int maxTime = 30;

	// Projectile =============================================================
	sf::Texture projectileTex;
	sf::Texture enemyProjectileTex;

	if (!projectileTex.loadFromFile("space_shooter_assets/player_laser.png"))
		throw "could not load projectile png";

	if (!enemyProjectileTex.loadFromFile("space_shooter_assets/enemy_laser.png"))
		throw "could not load projectile png";

	sf::Vector2f playerCenter;
	sf::Vector2f enemyCenter;
	int shootTimer = 0;
	int enemy3ShootTimer = 0;

	// Game Over ============================
	bool gameOver = false;
	bool win = false;
	sf::Texture overWin, overLost;
	sf::Sprite END;
	sf::Sprite winner;
	if (!overLost.loadFromFile("space_shooter_assets/gameover.png"))
		throw "could not load gameover.png";
	END.setTexture(overLost);

	if (!overWin.loadFromFile("space_shooter_assets/winner.png"))
		throw "could not load winner.png";
	winner.setTexture(overWin);
	winner.setScale(sf::Vector2f(2.f, 2.f));


	END.setPosition(window.getSize().x / 2 - END.getGlobalBounds().width + 160.f, window.getSize().y / 2 - END.getGlobalBounds().height + 160.f);
	winner.setPosition(sf::Vector2f(window.getSize().x + winner.getGlobalBounds().width, window.getSize().y - winner.getGlobalBounds().height + 20.f));

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
		// Runs the code for the whole game if both are false
		if (gameOver == false && win == false)
		{
			// Update

			// Level
			// Levels only increase till level 7
			if (levelTimer.getElapsedTime().asSeconds() >= maxTime && level != 7)
			{
				// Timer only increases from level 4
				if (level >= 4)
					maxTime += 30;
				levelTimer.restart();
				level++;
				// Clear all the enemies on the screen when at level 7
				if (level == 7)
				{
					enemies.clear();
				}
			}

			// POWER UP
			// Every 20s a powerUp is given
			if (static_cast<int>(powerUpTimer.getElapsedTime().asSeconds()) >= 20)
			{
				// true when powerUp has been generated
				if (generatedPowerUp == false)
				{
					generatedPowerUp = true;
					randomPowerUp = rand() % 4;
				}
				// Move the powerUp until it either leaves the screen or player intersects it
				if (!player.shape.getGlobalBounds().intersects(powerUps[randomPowerUp].getGlobalBounds()) && powerUps[randomPowerUp].getPosition().y < window.getSize().y)
				{
					powerUps[randomPowerUp].move(sf::Vector2f(0.f, 3.f));
				}
				else
				{
					// Take the power up back to above the screen
					powerUps[randomPowerUp].setPosition(sf::Vector2f(rand() % window.getSize().x, 0 - powerUps[randomPowerUp].getGlobalBounds().height));
					// Set it to false so a new powerUp can be generated later
					generatedPowerUp = false;
					switch (randomPowerUp)
					{
						// Double gun power up
					case 0:
						player.powerUp = 0;
						break;
						// Shield power up, true means sheild is on
					case 1:
						sheildOnOff = true;
						// set to 1 so that any previous powerUp is gone
						player.powerUp = 1;
						break;
						// Health powerUp
					case 2:
						player.hp += 15;
						if (player.hp >= player.maxHp)
							player.maxHp = player.hp;
						break;
						// Rocket powerUp
					case 3:
						player.rockets.push_back(Bullet(&rocketTex, sf::Vector2f(player.shape.getPosition().x + player.shape.getGlobalBounds().width / 2 - 3.5f, player.shape.getPosition().y + player.shape.getGlobalBounds().height / 2)));
						break;
					}
					powerUpTimer.restart();
				}
			}

			// Default normal gun
			if (player.powerUp != 0)
				playerCenter = sf::Vector2f(player.shape.getPosition().x + player.shape.getGlobalBounds().width / 2 - 3.5f, player.shape.getPosition().y + player.shape.getGlobalBounds().height / 2);
			// Double Gun (Left Gun Position)
			else if (player.powerUp == 0)
				playerCenter = sf::Vector2f(player.shape.getPosition().x + player.shape.getGlobalBounds().width / 2 - 20.f, player.shape.getPosition().y + player.shape.getGlobalBounds().height / 2);

			// Sheild Timer
			if (static_cast<int>(powerUpTimer.getElapsedTime().asSeconds()) >= 10 && sheildOnOff == true)
			{
				player.powerUp = -1;
				sheildOnOff = false;
			}

			// Count the number of rockets the player has
			rocketCount.setString(std::to_string(player.rockets.size()));

			// Enemies
			if (spawnTime <= 60)
				spawnTime++;
			
			// Rocks
			if (spawnTime >= 20 && level <= 3)
			{
				rock = rand() % 5 + 1;
				if (rock == 1)
					enemies.push_back(Enemy(&rock1Tex, 0, window.getSize(), 1.0f, 1, false, enemies));
				else if (rock == 2)
					enemies.push_back(Enemy(&rock2Tex, 0, window.getSize(), 1.1f, 2, false, enemies));
				else if (rock == 3 && level >= 2)
					enemies.push_back(Enemy(&rock3Tex, 0, window.getSize(), 1.15f, 3, false, enemies));
				else if (rock == 4 && level == 3)
					enemies.push_back(Enemy(&rock4Tex, 0, window.getSize(), 1.17f, 4, false, enemies));
				else if (rock == 5 && level == 3)
					enemies.push_back(Enemy(&rock5Tex, 0, window.getSize(), 1.2f, 5, false, enemies));
				spawnTime = 0;
			}

			// Enemy Ships
			if (spawnTime >= 60 && (level > 3 && level <= 6))
			{
				rock = (rand() % (level - 3)) + 4;
				if (rock == 4)
					enemies.push_back(Enemy(&enemy_1, 2, window.getSize(), 0.4f, 2, true, enemies));
				else if (rock == 5 && level >= 5)
					enemies.push_back(Enemy(&enemy_2, 3, window.getSize(), 0.6f, 4, true, enemies));
				else if (rock == 6 && level >= 6)
					enemies.push_back(Enemy(&enemy_3, 4, window.getSize(), 0.5f, 5, true, enemies));
				spawnTime = 0;
			}

			// BOSS SPAWN ====================================================================================
			if (level == 7 && boss[0].hp > 0)
			{
				// Boss Moves Till Defined Location
				if (boss[0].enemy.getPosition() != sf::Vector2f(boss[0].enemy.getPosition().x, 0 + boss[0].enemy.getGlobalBounds().height / 4) && bossStop == false)
					boss[0].enemy.move(0.f, 5.f);
				else
					bossStop = true;

				// True will make sure that move statement isnt executed anymore
				if (bossStop == true)
				{
					// Right Move
					if ((boss[0].enemy.getPosition().x < window.getSize().x - boss[0].enemy.getGlobalBounds().width) && dir == 0)
					{
						bossMove = 3.f;
						boss[0].enemy.move(sf::Vector2f(bossMove, 0));
					}
					else
						dir = 1;
					// Left Move
					if (boss[0].enemy.getPosition().x > 0 && dir == 1)
					{
						bossMove = -3.f;
						boss[0].enemy.move(sf::Vector2f(bossMove, 0));
					}
					else
						dir = 0;
				}

				// Lower Two Guns Firing
				if (LowerOrUpperGun == 0)
				{
					if (bossShotTimerLower <= 30)
						bossShotTimerLower++;

					if (bossShotTimerLower >= 30)
					{
						// Bottom Right Gun
						enemyCenter = sf::Vector2f(boss[0].enemy.getPosition().x + boss[0].enemy.getGlobalBounds().width / 2 + 29.f, boss[0].enemy.getPosition().y + boss[0].enemy.getGlobalBounds().height - 16.f);
						boss[0].bullets[0].push_back(Bullet(&enemyProjectileTex, enemyCenter));
						// Bottom Left Gun
						enemyCenter = sf::Vector2f(boss[0].enemy.getPosition().x + boss[0].enemy.getGlobalBounds().width / 4 + 5.f, boss[0].enemy.getPosition().y + boss[0].enemy.getGlobalBounds().height - 16.f);
						boss[0].bullets[1].push_back(Bullet(&enemyProjectileTex, enemyCenter));
						bossShotTimerLower = 0;
					}
					// Bullets fired from guns
					boss[0].bulletsFire(0, enemyCenter, bossMove, window.getSize(), &player);
					boss[0].bulletsFire(1, enemyCenter, bossMove, window.getSize(), &player);
					// If 5s passed, fire upper weapons
					if (LoF.getElapsedTime().asSeconds() >= 5)
					{
						// clear the vectors first
						boss[0].bullets[0].clear();
						boss[0].bullets[1].clear();
						// Restart the 5s timer
						LoF.restart();
						// Move to the fire upper weapons if condition
						LowerOrUpperGun = 1;
					}
				}

				// Upper Guns Firing
				if (LowerOrUpperGun == 1)
				{
					if (bossShotTimerUpper <= 30)
						bossShotTimerUpper++;

					if (bossShotTimerUpper >= 30)
					{
						// Upper Left First Gun
						enemyCenter = sf::Vector2f(boss[0].enemy.getPosition().x + 10.f, boss[0].enemy.getPosition().y + boss[0].enemy.getGlobalBounds().height / 3);
						boss[0].bullets[2].push_back(Bullet(&enemyProjectileTex, enemyCenter));
						// Upper Left Second Gun
						enemyCenter = sf::Vector2f(boss[0].enemy.getPosition().x + 25.f, boss[0].enemy.getPosition().y + boss[0].enemy.getGlobalBounds().height / 3 + 8.5f);
						boss[0].bullets[3].push_back(Bullet(&enemyProjectileTex, enemyCenter));
						// Upper Left Third Gun
						enemyCenter = sf::Vector2f(boss[0].enemy.getPosition().x + 40.f, boss[0].enemy.getPosition().y + boss[0].enemy.getGlobalBounds().height / 3 + 11.f);
						boss[0].bullets[4].push_back(Bullet(&enemyProjectileTex, enemyCenter));
						// Upper Right Third Gun
						enemyCenter = sf::Vector2f(boss[0].enemy.getPosition().x + boss[0].enemy.getGlobalBounds().width / 2 + 45.f, boss[0].enemy.getPosition().y + boss[0].enemy.getGlobalBounds().height / 3 + 11.f);
						boss[0].bullets[5].push_back(Bullet(&enemyProjectileTex, enemyCenter));
						// Upper Right Second Gun
						enemyCenter = sf::Vector2f(boss[0].enemy.getPosition().x + boss[0].enemy.getGlobalBounds().width / 2 + 60.f, boss[0].enemy.getPosition().y + boss[0].enemy.getGlobalBounds().height / 3 + 8.5f);
						boss[0].bullets[6].push_back(Bullet(&enemyProjectileTex, enemyCenter));
						// Upper Right First Gun
						enemyCenter = sf::Vector2f(boss[0].enemy.getPosition().x + boss[0].enemy.getGlobalBounds().width / 2 + 73.f, boss[0].enemy.getPosition().y + boss[0].enemy.getGlobalBounds().height / 3);
						boss[0].bullets[7].push_back(Bullet(&enemyProjectileTex, enemyCenter));
						bossShotTimerUpper = 0;
					}
					// Bullets fired from different guns
					boss[0].bulletsFire(2, enemyCenter, bossMove, window.getSize(), &player);
					boss[0].bulletsFire(3, enemyCenter, bossMove, window.getSize(), &player);
					boss[0].bulletsFire(4, enemyCenter, bossMove, window.getSize(), &player);
					boss[0].bulletsFire(5, enemyCenter, bossMove, window.getSize(), &player);
					boss[0].bulletsFire(6, enemyCenter, bossMove, window.getSize(), &player);
					boss[0].bulletsFire(7, enemyCenter, bossMove, window.getSize(), &player);
					if (LoF.getElapsedTime().asSeconds() >= 5)
					{
						for (size_t j = 2; j < boss[0].bullets.size(); j++)
						{
							boss[0].bullets[j].clear();
						}
						LoF.restart();
						// Move to the fire lower weapons if condition
						LowerOrUpperGun = 0;
					}
				}
				// Each time 20s elapse, clear all the bullets vectors
				if (static_cast<int>(BeamTimer.getElapsedTime().asSeconds()) == 20)
				{
					for (size_t i = 0; i < boss[0].bullets.size(); i++)
					{
						boss[0].bullets[i].clear();
					}
					time.restart();
				}
				// Fire the beam each time 20s elapse
				if (BeamTimer.getElapsedTime().asSeconds() >= 20)
				{
					// -1 means to not fire upper or lower guns at all
					LowerOrUpperGun = -1;
					// fire the beam for 5s, then fire lower guns and restart the BeamTimer 
					if (time.getElapsedTime().asSeconds() >= 5)
					{
						LowerOrUpperGun = 0;
						BeamTimer.restart();
					}
					// for shooting the beam
					else if (boss[0].enemyShootTimer <= 1)
						boss[0].enemyShootTimer++;

					if (boss[0].enemyShootTimer >= 1)
					{
						enemyCenter = sf::Vector2f(boss[0].enemy.getPosition().x + boss[0].enemy.getGlobalBounds().width / 2 - 6.f, boss[0].enemy.getPosition().y + boss[0].enemy.getGlobalBounds().height - 15.f);
						boss[0].bullets[0].push_back(Bullet(&enemyProjectileTex, enemyCenter));
						boss[0].bulletsFire(0, enemyCenter, bossMove, window.getSize(), &player);
						boss[0].enemyShootTimer = 0;
					}
				}
			}

			// if level isnt 7 then allow the non-boss enemies to spawn
			if (level != 7)
			{
				for (size_t i = 0; i < enemies.size(); i++)
				{
					// if enemies dont shoot, then move the rocks and dont shoot
					if (enemies[i].shoots == false)
					{
						enemies[i].enemy.move(0, 5.f);
						enemies[i].hp = 0;
						enemies[i].enemyHealthBar.setPosition(sf::Vector2f(0.f, -10.f));
						if (enemies[i].enemy.getPosition().y > window.getSize().y)
							enemies.erase(enemies.begin() + i);
					}
					else
					{
						// constantly move the enemy down the screen
						enemies[i].enemy.move(0, 2.f);
						// if enemy1 then shoot only one gun
						if (enemies[i].enemy.getTexture() == &enemy_1)
						{
							enemies[i].enemyHealthBar.setPosition(sf::Vector2f(enemies[i].enemy.getPosition().x + 9.f, enemies[i].enemy.getPosition().y - 6.f));
							enemyCenter = sf::Vector2f(enemies[i].enemy.getPosition().x + enemies[i].enemy.getGlobalBounds().width / 2 - 6.f, enemies[i].enemy.getPosition().y + enemies[i].enemy.getGlobalBounds().height / 2);
							if (enemies[i].enemyShootTimer <= 40)
								enemies[i].enemyShootTimer++;
							if (enemies[i].enemyShootTimer >= 40)
							{
								enemies[i].bullets[0].push_back(Bullet(&enemyProjectileTex, enemyCenter));
								enemies[i].enemyShootTimer = 0;
							}
						}
						// if enemy2 then shoot the second gun as well
						else if (enemies[i].enemy.getTexture() == &enemy_2)
						{
							enemies[i].enemyHealthBar.setPosition(sf::Vector2f(enemies[i].enemy.getPosition().x + 4.f, enemies[i].enemy.getPosition().y - 5.f));
							enemyCenter = sf::Vector2f(enemies[i].enemy.getPosition().x + enemies[i].enemy.getGlobalBounds().width / 2 + 6.f, enemies[i].enemy.getPosition().y + enemies[i].enemy.getGlobalBounds().height / 2);
							if (enemies[i].enemyShootTimer <= 25)
								enemies[i].enemyShootTimer++;
							if (enemies[i].enemyShootTimer >= 25)
							{
								enemies[i].bullets[0].push_back(Bullet(&enemyProjectileTex, enemyCenter));
								enemyCenter = sf::Vector2f(enemies[i].enemy.getPosition().x + enemies[i].enemy.getGlobalBounds().width / 2 - 18.f, enemies[i].enemy.getPosition().y + enemies[i].enemy.getGlobalBounds().height / 2);
								enemies[i].bullets[1].push_back(Bullet(&enemyProjectileTex, enemyCenter));
								enemies[i].enemyShootTimer = 0;
							}
						}
						// if enemy3 then shoot the third gun as well
						else if (enemies[i].enemy.getTexture() == &enemy_3)
						{
							enemies[i].enemyHealthBar.setPosition(sf::Vector2f(enemies[i].enemy.getPosition().x + 5.5f, enemies[i].enemy.getPosition().y - 5.f));
							enemyCenter = sf::Vector2f(enemies[i].enemy.getPosition().x + enemies[i].enemy.getGlobalBounds().width / 2 + 6.f, enemies[i].enemy.getPosition().y + enemies[i].enemy.getGlobalBounds().height / 2);
							if (enemies[i].enemyShootTimer <= 30)
								enemies[i].enemyShootTimer++;
							if (enemies[i].enemyShootTimer >= 30)
							{
								enemies[i].bullets[0].push_back(Bullet(&enemyProjectileTex, enemyCenter));
								enemyCenter = sf::Vector2f(enemies[i].enemy.getPosition().x + enemies[i].enemy.getGlobalBounds().width / 2 - 18.f, enemies[i].enemy.getPosition().y + enemies[i].enemy.getGlobalBounds().height / 2);
								enemies[i].bullets[1].push_back(Bullet(&enemyProjectileTex, enemyCenter));
								enemyCenter = sf::Vector2f(enemies[i].enemy.getPosition().x + enemies[i].enemy.getGlobalBounds().width / 2 - 6.f, enemies[i].enemy.getPosition().y + enemies[i].enemy.getGlobalBounds().height / 2);
								enemies[i].bullets[2].push_back(Bullet(&enemyProjectileTex, enemyCenter));
								enemies[i].enemyShootTimer = 0;
							}
						}
						// move the bullets outside from vectors depending on enemies
						enemies[i].bulletsFire(0, enemyCenter, 0.f, window.getSize(), &player);
						
						if (enemies[i].enemy.getTexture() == &enemy_2 || enemies[i].enemy.getTexture() == &enemy_3)
						{
							enemies[i].bulletsFire(1, enemyCenter, 0.f, window.getSize(), &player);
						}
						if (enemies[i].enemy.getTexture() == &enemy_3)
						{
							enemies[i].bulletsFire(2, enemyCenter, 0.f, window.getSize(), &player);
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
			// Setting player health beneath the player 
			playerHealthText.setPosition(sf::Vector2f(player.shape.getPosition().x + 20.f, player.shape.getPosition().y + player.shape.getGlobalBounds().height));
			// Shield powerUp has been taken by the player then place a sheild on the player
			if (player.powerUp == 1)
				sheildGlow.setPosition(player.shape.getPosition().x - 18.f, player.shape.getPosition().y - 15.4f);

			// This was how to make sure with mouse i dont move out of screen
			/*if (player.getPosition().x <= 0)
				player.setPosition(sf::Vector2f(0.f, player.getPosition().y));

			if (player.getPosition().x >= window.getSize().x - player.getGlobalBounds().width)
				player.setPosition(sf::Vector2f(window.getSize().x - player.getGlobalBounds().width, player.getPosition().y));*/

			// Projectile
			if (shootTimer <= 7)
				shootTimer++;

			// Generate the projectiles when timer is above or equals to 7 and lmb is pressed
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && shootTimer >= 7)
			{
				player.bullets[0].push_back(Bullet(&projectileTex, playerCenter));
				if (player.powerUp == 0)
				{
					playerCenter = sf::Vector2f(player.shape.getPosition().x + player.shape.getGlobalBounds().width - 24.f, player.shape.getPosition().y + player.shape.getGlobalBounds().height / 2);
					player.bullets[1].push_back(Bullet(&projectileTex, playerCenter));
				}
				shootTimer = 0;
			}

			// ROCKET
			// When R is pressed set rocketShot to true
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				rocketShot = true;
				// Set the rocket position on the player position before moving
				player.rockets.back().bullet.setPosition(sf::Vector2f(player.shape.getPosition().x + player.shape.getGlobalBounds().width / 2 - 3.5f, player.shape.getPosition().y + player.shape.getGlobalBounds().height / 2));
			}

			// Move the rocket when it is shot
			if (rocketShot == true)
			{
				player.rockets.back().bullet.move(sf::Vector2f(0, -5.f));
				// Delete if outside window
				if (player.rockets.back().bullet.getPosition().y < 0)
				{
					rocketShot = false;
					player.rockets.pop_back();
				}
				else if (level != 7)
				{
					// Delete when intersects and damage the opponent
					for (size_t j = 0; j < enemies.size(); j++)
					{
						if (player.rockets.back().bullet.getGlobalBounds().intersects(enemies[j].enemy.getGlobalBounds()))
						{
							player.rockets.pop_back();
							if (enemies[j].shoots == false)
							{
								score += enemies[j].points;
								enemies.erase(enemies.begin() + j);
								rocketShot = false;
							}
							else
							{
								enemies[j].hp -= 20;
								enemies[j].enemyHealthBar.setSize(sf::Vector2f((float)enemies[j].hp * 15.f, 5.f));
								if (enemies[j].hp <= 0)
								{
									score += enemies[j].points;
									enemies.erase(enemies.begin() + j);
								}
								rocketShot = false;
							}
							break;
						}
					}
				}
				else
				{
					if (player.rockets.back().bullet.getGlobalBounds().intersects(boss[0].enemy.getGlobalBounds()))
					{
						player.rockets.pop_back();
						rocketShot = false;
						boss[0].hp-= 20;
						boss[0].enemyHealthBar.setSize(sf::Vector2f((float)boss[0].hp * 5.f, 40.f));
						if (boss[0].hp <= 0)
						{
							score += boss[0].points;
							boss.erase(boss.begin() + 0);
							win = true;
							break;
						}
					}
				}
			}

			// Collision and OutOfBounds
			for (size_t k = 0; k < player.bullets.size(); k++)
			{
				for (size_t i = 0; i < player.bullets[k].size(); i++)
				{
					player.bullets[k][i].bullet.move(sf::Vector2f(0, -10.f));
					if (player.bullets[k][i].bullet.getPosition().y < 0)
					{
						player.bullets[k].erase(player.bullets[k].begin() + i);
					}
					else 
					{
						// if not level 7 then determine the non-boss enemy collisions
						if (level != 7)
						{
							for (size_t j = 0; j < enemies.size(); j++)
							{
								// Bullet on rock collision
								if (player.bullets[k][i].bullet.getGlobalBounds().intersects(enemies[j].enemy.getGlobalBounds()) && enemies[j].shoots == false)
								{
									score += enemies[j].points;
									player.bullets[k].erase(player.bullets[k].begin() + i);
									enemies.erase(enemies.begin() + j);
									break;
								}
								// Bullet on ship collision
								else if (player.bullets[k][i].bullet.getGlobalBounds().intersects(enemies[j].enemy.getGlobalBounds()) && enemies[j].shoots == true)
								{
									player.bullets[k].erase(player.bullets[k].begin() + i);
									enemies[j].hp--;
									enemies[j].enemyHealthBar.setSize(sf::Vector2f((float)enemies[j].hp * 15.f, 5.f));
									if (enemies[j].hp <= 0)
									{
										score += enemies[j].points;
										enemies.erase(enemies.begin() + j);
									}
									break;
								}
							}
						}
						else
						{
							// Bullet on boss collision
							if (player.bullets[k][i].bullet.getGlobalBounds().intersects(boss[0].enemy.getGlobalBounds()))
							{
								player.bullets[k].erase(player.bullets[k].begin() + i);
								boss[0].hp--;
								boss[0].enemyHealthBar.setSize(sf::Vector2f((float)boss[0].hp * 5.f, 40.f));
								if (boss[0].hp <= 0)
								{
									score += boss[0].points;
									boss.erase(boss.begin() + 0);
									win = true;
									break;
								}
							}
						}
					}
				}
			}

			// Enemy collision
			// if not level 7 then handle enemy on player collision
			if (level != 7)
			{
				for (size_t i = 0; i < enemies.size(); i++)
				{
					if (player.shape.getGlobalBounds().intersects(enemies[i].enemy.getGlobalBounds()))
					{
						enemies.erase(enemies.begin() + i);
						if (player.powerUp != 1)
							player.hp--;
						break;
					}
				}
			}
			// player on boss collision
			else if (!boss.empty())
			{
				if (player.shape.getGlobalBounds().intersects(boss[0].enemy.getGlobalBounds()))
				{
					boss[0].hp--;
					boss[0].enemyHealthBar.setSize(sf::Vector2f((float)boss[0].hp * 5.f, 40.f));
					if (player.powerUp != 1)
						player.hp--;
					if (boss[0].hp <= 0)
					{
						boss.erase(boss.begin());
						win = true;
					}
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
		window.draw(rocketIcon);
		window.draw(rocketCount);

		window.draw(player.shape);

		// Draw enemies and their bullets and healthBar
		for (size_t i = 0; i < enemies.size(); i++)
		{
			window.draw(enemies[i].enemy);
			window.draw(enemies[i].enemyHealthBar);
			for (size_t j = 0; j < enemies[i].bullets.size(); j++)
			{
				for (size_t k = 0; k < enemies[i].bullets[j].size(); k++)
				{
					window.draw(enemies[i].bullets[j][k].bullet);
				}
			}
		}

		// Draw Player bullets and Rocket If Shot
		for (size_t i = 0; i < player.bullets.size(); i++)
		{
			for (size_t j = 0; j < player.bullets[i].size(); j++)
			{
				window.draw(player.bullets[i][j].bullet);
			}
			if (rocketShot == true)
				window.draw(player.rockets.back().bullet);
			
		}

		// Draw the score of the player
		window.draw(text);

		// Only set timer when game is running
		if (gameOver == false && win == false)
		{
			text.setString("Timer: " + std::to_string(static_cast<int>(levelTimer.getElapsedTime().asSeconds())));
			text.setPosition(window.getSize().x - text.getGlobalBounds().width, 0.f);
			// If a powerUp has been generated then draw it
			if (generatedPowerUp == true)
				window.draw(powerUps[randomPowerUp]);
			// Draw the sheildGlow is the sheild is there
			if (player.powerUp == 1)
				window.draw(sheildGlow);
		}
		// Draw the timer
		window.draw(text);

		// Set Level Text and Draw it
		text.setString("Level " + std::to_string(level));
		text.setPosition(window.getSize().x / 2 - text.getGlobalBounds().width - 10, window.getSize().y - text.getGlobalBounds().height * 2);
		window.draw(text);

		// Draw Player Health Text
		window.draw(playerHealthText);

		// If not won then keep drawing the boss
		if (win == false)
			window.draw(boss[0].enemy);

		// If boss has stopped draw his health bar and the bullets it shoots
		if (bossStop == true && !boss.empty())
		{
			window.draw(boss[0].enemyHealthBar);
			for (size_t i = 0; i < boss[0].bullets.size(); i++)
			{
				for (size_t j = 0; j < boss[0].bullets[i].size(); j++)
				{
					window.draw(boss[0].bullets[i][j].bullet);
				}
			}
		}

		// If gameOver then draw the loss screen
		if (gameOver == true)
		{
			window.draw(END);
		}

		// if won then slowly move the win screen to the middle and draw it
		else if (win == true)
		{
			if (winner.getPosition() != sf::Vector2f(window.getSize().x - winner.getGlobalBounds().width, window.getSize().y - winner.getGlobalBounds().height + 20.f))
				winner.move(-20.f, 0.f);

			std::string scoreString = "Score: " + std::to_string(score);
			text.setString(scoreString);
			text.setPosition(0.f, 0.f);
			window.draw(text);
			window.draw(winner);
		}
		// Display
		window.display();
	}
}
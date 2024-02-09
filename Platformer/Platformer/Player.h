#pragma once

enum PLAYER_ANIMATION_STATE { IDLE = 0, MOVING_LEFT, MOVING_RIGHT, JUMPING, FALLING };

class Player
{
private:
	sf::Sprite sprite;
	sf::Texture textureSheet;
	sf::Clock animation_timer;

	// Animation
	short animState;
	sf::IntRect currentFrame;
	bool animationSwitch;

	// Physics
	sf::Vector2f velocity;
	float velocityMax;
	float velocityMin;
	float acceleration;
	float drag;
	float gravity;
	float velocityMaxY;

	// Movement

	// Core

	void initVariables();
	void initTexture();
	void initSprite();
	void initAnimations();
	void initPhysics();
public:
	Player();
	~Player();

	// Accessors
	const bool& getAnimSwitch();
	const sf::Vector2f getPosition() const;
	const sf::FloatRect& getBounds() const;

	// Modifiers
	void setPosition(const float x, const float y);
	void resetVelocityY();

	// Functions
	void resetAnimationTimer();
	void move(const float dir_x, const float dir_y);
	void updatePhysics();
	void updateMovement();
	void updateAnimations();
	void update();
	void render(sf::RenderTarget& target);
};


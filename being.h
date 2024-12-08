#pragma once
#include "asset_manager.h"
#include "settings.h"
#include "animation_manager.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class Being
{
public:
	AnimationManager animationManager;
	
	AssetManager* myTexture;

	sf::Sprite mySprite;

	float moveSpeed;

	int currentHealth;
	int maxHealth;

	std::string beingType;

	Being();

	Being(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID);

	void create(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID);

	// this is used after the being create() since
	// the texture rect is not set until after.
	void createSpriteAndRect(sf::Vector2f position, settings& settings, std::string beingType);

	sf::RectangleShape getCollisionRect();

	double getID();

	void draw(sf::RenderWindow& window, bool showRect);

	void update(settings& settings);

	void changeAcceleration(sf::Vector2f amount);

	void multAcceleration(sf::Vector2f amount);

	void setAcceleration(sf::Vector2f amount);

	sf::Vector2f getAcceleration();

	void walk(sf::Vector2f moveVector);

	void walkAtAngle(int rotation);

	void jumpWalk(int rotation, float distance);

	void setTextureRect(sf::IntRect newRect);

	float getTargetRotation();

	void setTargetRotation(float newValue);

	void updateRotation(settings& settings);
private:
	sf::Vector2f velocity;
	sf::Vector2f acceleration;

	sf::RectangleShape collisionRect;
	sf::RectangleShape spriteRect;

	double myID;

	float targetRotation;

	float rotation;
};


#pragma once
#include "being.h"
#include "get_triangle_points.h"
#include <array>

class Enemy : public Being
{
public:
	std::array<sf::Vector2f, 3> sightTriangle;

	std::vector<Being*> collidedBeings;

	sf::Sprite* target;

	float followRadius;

	std::array<float, 3> sightLengthWidthLimit;

	float cubeJumpTimer;

	Enemy();
	
	Enemy(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID);

	void createEnemy(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID);

	void drawRects(sf::RenderWindow& window);

	void updateEnemy(settings& settings, sf::RenderWindow& window);

	void updateSightTri();

	void updateTarget(sf::RenderWindow& window, settings& settings);

	void followTarget(sf::RenderWindow& window, settings& settings);

	void patrol();
};

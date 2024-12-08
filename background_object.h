#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "get_pos_from_raycast.h"

class BackgroundObject
{
public:
	std::array<sf::Vertex, 4> vertices;

	sf::Vector2f dimensions;

	sf::Vector2f pos;

	float width;

	float rotation;

	BackgroundObject();

	// TODO: allow scaling to different ratios?
	// Note: order of texCoords has to be tl, tr, br, bl
	BackgroundObject(sf::Vector2f pos, float width, int rotation, std::array<sf::Vector2u, 4> texCoords);

	void create(sf::Vector2f pos, float width, int rotation, std::array<sf::Vector2u, 4> texCoords);
};


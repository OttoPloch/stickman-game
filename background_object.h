#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "get_pos_from_raycast.h"
#include <iostream>

class BackgroundObject
{
public:
	std::array<sf::Vertex, 4> vertices;

	std::array<sf::Vertex, 4> unrotatedVertices;

	sf::Vector2f dimensions;

	sf::Vector2f pos;

	// top, left, right, bottom
	//std::array<float, 4> boundingBox;

	//std::array<sf::Vector2f, 4> orderedVertices;

	float width;

	float height;

	float yRatio;

	float rotation;

	BackgroundObject();

	// TODO: allow scaling to different ratios?
	// Note: order of texCoords has to be tl, tr, br, bl
	BackgroundObject(sf::Vector2f pos, float width, int rotation, std::array<sf::Vector2u, 4> texCoords);

	void create(sf::Vector2f pos, float width, int rotation, std::array<sf::Vector2u, 4> texCoords);
};


#pragma once
#include "background_object.h"

class Paper : public BackgroundObject
{
public:
	Paper(sf::Vector2f pos, float width, int rotation, std::array<sf::Vector2u, 4> texCoords);
};


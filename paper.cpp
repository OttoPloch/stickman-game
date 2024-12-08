#include "paper.h"

Paper::Paper(sf::Vector2f pos, float width, int rotation, std::array<sf::Vector2u, 4> texCoords) : BackgroundObject(pos, width, rotation, texCoords) {}
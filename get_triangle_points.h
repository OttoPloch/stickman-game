#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "get_pos_from_raycast.h"

std::array<sf::Vector2f, 3> getTriPoints(sf::Vector2f startPos, float length, float width, float rotation);
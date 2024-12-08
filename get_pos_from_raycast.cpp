#include "get_pos_from_raycast.h"

sf::Vector2f getPosFromRaycast(sf::Vector2f startPos, int length, int rotation) {
	sf::Vector2f resultPos;

	resultPos.x = startPos.x + (std::sin(rotation / 57.29578f) * length);
	resultPos.y = startPos.y + (std::cos(rotation / 57.29578f) * -length);

	return resultPos;
}
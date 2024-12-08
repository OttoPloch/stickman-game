#include "get_triangle_points.h"

std::array<sf::Vector2f, 3> getTriPoints(sf::Vector2f startPos, float length, float width, float rotation) {
	// - - - - - - - - - -
	// - - - - - - - - - -
	// - b * - - - - - - -
	// - - - - - - - * c -
	// - - - - - - - - - -
	// - - - - - - - - - -
	// - - - - - - - - - -
	// - - - * a - - - - -
	// - - - - - - - - - -
	// - - - - - - - - - -
	// a is the start pos, b and c are
	// the points we are trying to get.
	
	
	// the point where the bisector of
	// the start point angle and the line
	// formed by the two other resulting points meet.
	sf::Vector2f lineIntersection(getPosFromRaycast(startPos, length, rotation));

	sf::Vector2f a(startPos);
	sf::Vector2f b(getPosFromRaycast(lineIntersection, width / 2.f, 90 + rotation));
	sf::Vector2f c(getPosFromRaycast(lineIntersection, width / 2.f, -90 + rotation));

	//std::cout << "start: " << startPos.x << ", " << startPos.y << "; b: " << b.x << ", " << b.y << "; c: " << c.x << ", " << c.y << '\n';

	return {a, b, c};
}
#include "triangle_collide.h"

bool triangleCollide(std::array<sf::Vector2f, 3> triPoints, sf::Vector2f collidePoint) {
	float x1 = triPoints[0].x;
	float x2 = triPoints[1].x;
	float x3 = triPoints[2].x;

	float y1 = triPoints[0].y;
	float y2 = triPoints[1].y;
	float y3 = triPoints[2].y;

	float px = collidePoint.x;
	float py = collidePoint.y;
	
	float areaOrig = abs((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1));

	float area1 = abs((x1 - px) * (y2 - py) - (x2 - px) * (y1 - py));
	float area2 = abs((x2 - px) * (y3 - py) - (x3 - px) * (y2 - py));
	float area3 = abs((x3 - px) * (y1 - py) - (x1 - px) * (y3 - py));

	if (area1 + area2 + area3 == areaOrig) {
		return true;
	}

	return false;
}

bool triangleCollide(std::array<sf::Vector2f, 3> triPoints, sf::RectangleShape collideRect) {
	float x1 = triPoints[0].x;
	float x2 = triPoints[1].x;
	float x3 = triPoints[2].x;

	float triXPoints[] = { x1, x2, x3 };

	float y1 = triPoints[0].y;
	float y2 = triPoints[1].y;
	float y3 = triPoints[2].y;

	float triYPoints[] = { y1, y2, y3 };

	float rx = collideRect.getGlobalBounds().left;
	float ry = collideRect.getGlobalBounds().top;

	float rw = collideRect.getGlobalBounds().width;
	float rh = collideRect.getGlobalBounds().height;

	bool triPointInRect = false;
	bool rectPointInTri = false;

	for (int i = 0; i < 3; i++) {
		if (triXPoints[i] > rx && triXPoints[i] < rx + rw) {
			if (triYPoints[i] > ry && triYPoints[i] < ry + rh) {
				triPointInRect = true;
			}
		}
	}

	if (triangleCollide(triPoints, sf::Vector2f(rx, ry))) {
		rectPointInTri = true;
	}

	if (triangleCollide(triPoints, sf::Vector2f(rx + rw, ry))) {
		rectPointInTri = true;
	}

	if (triangleCollide(triPoints, sf::Vector2f(rx, ry + rh))) {
		rectPointInTri = true;
	}

	if (triangleCollide(triPoints, sf::Vector2f(rx + rw, ry + rh))) {
		rectPointInTri = true;
	}

	if (triPointInRect || rectPointInTri) {
		return true;
	}

	return false;
}

bool triangleCollide(std::array<sf::Vector2f, 3> triPoints, sf::IntRect collideRect) {
	float x1 = triPoints[0].x;
	float x2 = triPoints[1].x;
	float x3 = triPoints[2].x;

	float triXPoints[] = { x1, x2, x3 };

	float y1 = triPoints[0].y;
	float y2 = triPoints[1].y;
	float y3 = triPoints[2].y;

	float triYPoints[] = { y1, y2, y3 };

	float rx = collideRect.left;
	float ry = collideRect.top;

	float rw = collideRect.width;
	float rh = collideRect.height;

	bool triPointInRect = false;
	bool rectPointInTri = false;

	for (int i = 0; i < 3; i++) {
		if (triXPoints[i] > rx && triXPoints[i] < rx + rw) {
			if (triYPoints[i] > ry && triYPoints[i] < ry + rh) {
				triPointInRect = true;
			}
		}
	}

	if (triangleCollide(triPoints, sf::Vector2f(rx, ry))) {
		rectPointInTri = true;
	}

	if (triangleCollide(triPoints, sf::Vector2f(rx + rw, ry))) {
		rectPointInTri = true;
	}

	if (triangleCollide(triPoints, sf::Vector2f(rx, ry + rh))) {
		rectPointInTri = true;
	}

	if (triangleCollide(triPoints, sf::Vector2f(rx + rw, ry + rh))) {
		rectPointInTri = true;
	}

	if (triPointInRect || rectPointInTri) {
		return true;
	}

	return false;
}

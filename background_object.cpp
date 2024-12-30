#include "background_object.h"

BackgroundObject::BackgroundObject() : rotation(0.f), width(0.f) {}

BackgroundObject::BackgroundObject(sf::Vector2f pos, float width, int rotation, std::array<sf::Vector2u, 4> texCoords) {
	create(pos, width, rotation, texCoords);
}

void BackgroundObject::create(sf::Vector2f pos, float width, int rotation, std::array<sf::Vector2u, 4> texCoords) {
	yRatio = 1.f * texCoords[2].y / texCoords[2].x;

	dimensions = sf::Vector2f(width, width * yRatio);
	
	height = dimensions.y;

	vertices[0] = sf::Vertex(pos, sf::Vector2f(texCoords[0].x, texCoords[0].y));
	vertices[1] = sf::Vertex(sf::Vector2f(pos.x + dimensions.x, pos.y), sf::Vector2f(texCoords[1].x, texCoords[1].y));
	vertices[2] = sf::Vertex(sf::Vector2f(pos.x + dimensions.x, pos.y + dimensions.y), sf::Vector2f(texCoords[2].x, texCoords[2].y));
	vertices[3] = sf::Vertex(sf::Vector2f(pos.x, pos.y + dimensions.y), sf::Vector2f(texCoords[3].x, texCoords[3].y));

	vertices[1].position = getPosFromRaycast(vertices[0].position, dimensions.x, 90 + rotation);
	vertices[2].position = getPosFromRaycast(vertices[1].position, dimensions.y, 180 + rotation);
	vertices[3].position = getPosFromRaycast(vertices[2].position, dimensions.x, 270 + rotation);

	unrotatedVertices[0] = sf::Vertex(pos, sf::Vector2f(texCoords[0].x, texCoords[0].y));
	unrotatedVertices[1] = sf::Vertex(sf::Vector2f(pos.x + dimensions.x, pos.y), sf::Vector2f(texCoords[1].x, texCoords[1].y));
	unrotatedVertices[2] = sf::Vertex(sf::Vector2f(pos.x + dimensions.x, pos.y + dimensions.y), sf::Vector2f(texCoords[2].x, texCoords[2].y));
	unrotatedVertices[3] = sf::Vertex(sf::Vector2f(pos.x, pos.y + dimensions.y), sf::Vector2f(texCoords[3].x, texCoords[3].y));

	//float topVal = vertices[0].position.y;
	//orderedVertices[0] = vertices[0].position;

	//for (int i = 1; i < 4; i++) {
	//	if (vertices[i].position.y < topVal) {
	//		topVal = vertices[i].position.y;
	//		orderedVertices[0] = vertices[i].position;
	//	}
	//}

	//float leftVal = vertices[0].position.x;
	//orderedVertices[1] = vertices[0].position;

	//for (int i = 1; i < 4; i++) {
	//	if (vertices[i].position.x < leftVal) {
	//		leftVal = vertices[i].position.x;
	//		orderedVertices[1] = vertices[i].position;
	//	}
	//}

	//float rightVal = vertices[0].position.x;
	//orderedVertices[2] = vertices[0].position;

	//for (int i = 1; i < 4; i++) {
	//	if (vertices[i].position.x > rightVal) {
	//		rightVal = vertices[i].position.x;
	//		orderedVertices[2] = vertices[i].position;
	//	}
	//}

	//float bottomVal = vertices[0].position.y;
	//orderedVertices[3] = vertices[0].position;

	//for (int i = 1; i < 4; i++) {
	//	if (vertices[i].position.y > bottomVal) {
	//		bottomVal = vertices[i].position.y;
	//		orderedVertices[3] = vertices[i].position;
	//	}
	//}

	//boundingBox = { topVal, leftVal, rightVal, bottomVal };

	this->pos = pos;
	this->width = width;
	this->rotation = rotation;
}
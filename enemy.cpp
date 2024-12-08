#include "enemy.h"

#include <cmath>

Enemy::Enemy() : target(nullptr), followRadius(100.f), sightLengthWidthLimit({100.f, 100.f, 100.f}), cubeJumpTimer(0.f) {}

Enemy::Enemy(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID) {
	createEnemy(position, textureManager, settings, beingType, ID);
}

void Enemy::createEnemy(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID) {
	create(position, textureManager, settings, beingType, ID);

	mySprite.setTextureRect(sf::IntRect(0, 0, settings.spriteSizes[beingType].x, settings.spriteSizes[beingType].y));

	createSpriteAndRect(position, settings, beingType);

	sightLengthWidthLimit = settings.sightLengthsWidthsLimits[beingType];

	updateSightTri();

	target = nullptr;

	followRadius = sightLengthWidthLimit[2];

	cubeJumpTimer = 0.f;
}

void Enemy::drawRects(sf::RenderWindow& window) {
	std::array<sf::Vertex, 4> triLines;

	triLines[0] = sf::Vertex(sightTriangle[0], sf::Color::Blue);
	triLines[1] = sf::Vertex(sightTriangle[1], sf::Color::Blue);
	triLines[2] = sf::Vertex(sightTriangle[2], sf::Color::Blue);
	triLines[3] = sf::Vertex(sightTriangle[0], sf::Color::Blue);

	window.draw(&triLines[0], triLines.size(), sf::LineStrip);

	sf::CircleShape circle(followRadius);
	circle.setOrigin(followRadius, followRadius);
	circle.setPosition(mySprite.getPosition());

	circle.setFillColor(sf::Color::Transparent);
	circle.setOutlineColor(sf::Color::Blue);
	circle.setOutlineThickness(1.f);

	window.draw(circle);
}

void Enemy::updateEnemy(settings& settings, sf::RenderWindow& window) {
	update(settings);

	updateSightTri();

	updateTarget(window, settings);
}

void Enemy::updateSightTri() {
	sightTriangle = getTriPoints(mySprite.getPosition(), sightLengthWidthLimit[0], sightLengthWidthLimit[1], mySprite.getRotation());
}

void Enemy::updateTarget(sf::RenderWindow& window, settings& settings) {
	if (collidedBeings.size() > 0) {
		for (int i = 0; i < collidedBeings.size(); i++) {
			if (collidedBeings[i]->beingType == "player" && target == nullptr) {
				target = &collidedBeings[i]->mySprite;
			}
		}
	}

	if (target != nullptr) {
		if (sqrt(pow(mySprite.getPosition().x - target->getPosition().x, 2) + pow(mySprite.getPosition().y - target->getPosition().y, 2)) > followRadius) {
			target = nullptr;

			if (beingType == "enemy_cube") {
				cubeJumpTimer = 0.f;
			}
		}
		else {
			followTarget(window, settings);
		}
	}
	else {
		patrol();
	}
}

void Enemy::followTarget(sf::RenderWindow& window, settings& settings) {
	if (sqrt(pow(mySprite.getPosition().x - target->getPosition().x, 2) + pow(mySprite.getPosition().y - target->getPosition().y, 2)) >= 20.f) {
		if (beingType == "enemy_ghoul") {
			sf::Vector2f direction = target->getPosition() - mySprite.getPosition();

			float angle = atan2(direction.y, direction.x) * 57.29578f + 90;

			if ((target->getPosition().x < mySprite.getPosition().x && target->getPosition().y > mySprite.getPosition().y) || (target->getPosition().x > mySprite.getPosition().x && target->getPosition().y < mySprite.getPosition().y)) {
				setTargetRotation(340.f);
			}
			else if ((target->getPosition().x < mySprite.getPosition().x && target->getPosition().y < mySprite.getPosition().y) || (target->getPosition().x > mySprite.getPosition().x && target->getPosition().y > mySprite.getPosition().y)) {
				setTargetRotation(20.f);
			}
			else {
				setTargetRotation(0.f);
			}

			walkAtAngle(angle);
		}
		else if (beingType == "enemy_cube") {
			setTargetRotation(0.f);

			sf::Vector2f direction = target->getPosition() - mySprite.getPosition();

			float angle = atan2(direction.y, direction.x) * 57.29578f + 90;

			if (cubeJumpTimer >= settings.cubeJumpSpeed) {
				jumpWalk(angle, settings.cubeJumpDistance);
				cubeJumpTimer = 0.f;
			}
			else {
				cubeJumpTimer += 1.f / settings.ticksPerSecond;
			}
		}
	}
	else {

	}
}

void Enemy::patrol() {
	setTargetRotation(mySprite.getRotation() + 4);
}
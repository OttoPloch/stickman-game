#include "player.h"

Player::Player() {}

Player::Player(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID) {
	createPlayer(position, textureManager, settings, beingType, ID);
}

void Player::createPlayer(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID) {
	create(position, textureManager, settings, beingType, ID);

	mySprite.setTextureRect(sf::IntRect(0, 0, settings.spriteSizes["player"].x, settings.spriteSizes["player"].y));

	createSpriteAndRect(position, settings, beingType);

	std::vector<sf::IntRect> stationaryFrames;
	std::vector<sf::IntRect> upFrames;
	std::vector<sf::IntRect> downFrames;
	std::vector<sf::IntRect> leftFrames;
	std::vector<sf::IntRect> rightFrames;

	stationaryFrames.push_back(sf::IntRect(0, 0, 30, 80));
	stationaryFrames.push_back(sf::IntRect(30, 0, 30, 80));

	upFrames.push_back(sf::IntRect(0, 80, 30, 80));
	upFrames.push_back(sf::IntRect(30, 80, 30, 80));
	upFrames.push_back(sf::IntRect(60, 80, 30, 80));
	upFrames.push_back(sf::IntRect(90, 80, 30, 80));

	downFrames.push_back(sf::IntRect(0, 160, 30, 80));
	downFrames.push_back(sf::IntRect(30, 160, 30, 80));
	downFrames.push_back(sf::IntRect(60, 160, 30, 80));
	downFrames.push_back(sf::IntRect(90, 160, 30, 80));

	leftFrames.push_back(sf::IntRect(0, 240, 30, 80));
	leftFrames.push_back(sf::IntRect(30, 240, 30, 80));
	leftFrames.push_back(sf::IntRect(60, 240, 30, 80));
	leftFrames.push_back(sf::IntRect(90, 240, 30, 80));

	rightFrames.push_back(sf::IntRect(0, 320, 30, 80));
	rightFrames.push_back(sf::IntRect(30, 320, 30, 80));
	rightFrames.push_back(sf::IntRect(60, 320, 30, 80));
	rightFrames.push_back(sf::IntRect(90, 320, 30, 80));

	animationManager.addAnimation("stationary", 2, stationaryFrames, 0.7f);
	animationManager.addAnimation("up", 4, upFrames, 0.3f);
	animationManager.addAnimation("down", 4, downFrames, 0.3f);
	animationManager.addAnimation("left", 4, leftFrames, 0.3f);
	animationManager.addAnimation("right", 4, rightFrames, 0.3f);
}
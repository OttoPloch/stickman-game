#include "being.h"

Being::Being() : moveSpeed(10.f), currentHealth(10), maxHealth(10), myID(-1), targetRotation(0.f), myTexture(nullptr), rotation(0.f) {}

Being::Being(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID) {
	create(position, textureManager, settings, beingType, ID);
}

void Being::create(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID) {
	myTexture = &textureManager;
	
	velocity.x = 0.f;
	velocity.y = 0.f;

	acceleration.x = 0.f;
	acceleration.y = 0.f;

	moveSpeed = settings.moveSpeeds[beingType];

	maxHealth = settings.maxHealths[beingType];
	currentHealth = maxHealth;

	this->beingType = beingType;

	myID = ID;

	targetRotation = 0.f;
}

void Being::createSpriteAndRect(sf::Vector2f position, settings& settings, std::string beingType) {
	mySprite.setOrigin(mySprite.getLocalBounds().width / 2.f, mySprite.getLocalBounds().height / 2.f);
	mySprite.setPosition(position);
	mySprite.setTexture(myTexture->texture);

	collisionRect.setSize(sf::Vector2f(settings.collisionRectSizes[beingType][0], settings.collisionRectSizes[beingType][1]));
	collisionRect.setOrigin(collisionRect.getLocalBounds().width / 2.f, collisionRect.getLocalBounds().height / 2.f);
	collisionRect.setPosition(mySprite.getPosition());

	collisionRect.setOutlineColor(sf::Color::Red);
	collisionRect.setOutlineThickness(2.f);
	collisionRect.setFillColor(sf::Color::Transparent);

	spriteRect.setSize(sf::Vector2f(mySprite.getLocalBounds().width, mySprite.getLocalBounds().height));
	spriteRect.setOrigin(spriteRect.getLocalBounds().width / 2.f, spriteRect.getLocalBounds().height / 2.f);
	spriteRect.setPosition(mySprite.getPosition());

	spriteRect.setOutlineColor(sf::Color::Green);
	spriteRect.setOutlineThickness(3.f);
	spriteRect.setFillColor(sf::Color::Transparent);
}

sf::RectangleShape Being::getCollisionRect() {
	return collisionRect;
}

double Being::getID() {
	return myID;
}

void Being::draw(sf::RenderWindow& window, bool showRect) {
	window.draw(mySprite);

	if (showRect) {
		window.draw(collisionRect);

		window.draw(spriteRect);
	}
}

void Being::update(settings& settings) {
	collisionRect.setPosition(mySprite.getPosition());
	spriteRect.setPosition(mySprite.getPosition());

	velocity.x += acceleration.x / settings.ticksPerSecond;
	velocity.y += acceleration.y / settings.ticksPerSecond;
	
	mySprite.move(velocity);

	velocity.x = 0.f;
	velocity.y = 0.f;

	float friction = settings.paperFriction;

	if (std::abs(getAcceleration().x) > 30.f) {
		int p = 0;

		getAcceleration().x > 0.f ? p = -1 : p = 1;

		changeAcceleration(sf::Vector2f(p * friction / settings.ticksPerSecond, 0));
	}
	if (std::abs(getAcceleration().y) > 30.f) {
		int p = 0;

		getAcceleration().y > 0.f ? p = -1 : p = 1;

		changeAcceleration(sf::Vector2f(0, p * friction / settings.ticksPerSecond));
	}

	if (std::abs(getAcceleration().x) <= 30.f) {
		setAcceleration(sf::Vector2f(0.f, getAcceleration().y));
	}
	if (std::abs(getAcceleration().y) <= 30.f) {
		setAcceleration(sf::Vector2f(getAcceleration().x, 0.f));
	}

	if (getAcceleration().x == 0.f && getAcceleration().y == 0.f) {
		if (animationManager.animations.size() > 0) {
			if (animationManager.currentAnimation != "stationary") {
				animationManager.reset();
				animationManager.currentAnimation = "stationary";
			}
			else {
				animationManager.timePassed += 1.f / settings.ticksPerSecond;
			}
		}
	}
	else {
		animationManager.timePassed += 1.f / settings.ticksPerSecond;
	}

	if (animationManager.animations.size() > 0) {
		if (animationManager.timePassed >= animationManager.animations[animationManager.currentAnimation].timeBetweenFrames) {
			animationManager.step();

			setTextureRect(animationManager.animations[animationManager.currentAnimation].currentFrameRect);
		}

		if (animationManager.animations[animationManager.currentAnimation].currentFrame == 0 && animationManager.timePassed == 1.f / settings.ticksPerSecond) {
			setTextureRect(animationManager.animations[animationManager.currentAnimation].currentFrameRect);
		}
	}

	updateRotation(settings);
}

void Being::changeAcceleration(sf::Vector2f amount) {
	acceleration.x += amount.x;
	acceleration.y += amount.y;
}

void Being::multAcceleration(sf::Vector2f amount) {
	acceleration.x *= amount.x;
	acceleration.y *= amount.y;
}

void Being::setAcceleration(sf::Vector2f amount) {
	acceleration.x = amount.x;
	acceleration.y = amount.y;
}

sf::Vector2f Being::getAcceleration() {
	return acceleration;
}

void Being::walk(sf::Vector2f moveVector) {
	if (abs(moveVector.x) == 1 && abs(moveVector.y) == 1) {
		setAcceleration(sf::Vector2f(moveVector.x * 0.707f * moveSpeed, moveVector.y * 0.707f * moveSpeed));
	}
	else {
		setAcceleration(sf::Vector2f(moveVector.x * moveSpeed, moveVector.y * moveSpeed));
	}

	if (moveVector.x != 0 || moveVector.y != 0) {
		if (animationManager.animations.size() > 0) {
			if (animationManager.currentAnimation == "stationary") {
				animationManager.reset();
			}
		}
	}

	moveVector.x > 0 ? animationManager.currentAnimation = "right" : animationManager.currentAnimation = "left";

	if (moveVector.y != 0) {
		moveVector.y > 0 ? animationManager.currentAnimation = "down" : animationManager.currentAnimation = "up";
	}
}

void Being::walkAtAngle(int rotation) {
	float xAcceleration = 0.f;
	float yAcceleration = 0.f;

	// rotation is modified like
	// that because i prefer it.
	xAcceleration = std::sin((-1 * rotation - 180) / 57.29578f);
	yAcceleration = std::cos((-1 * rotation - 180) / -57.29578f);

	setAcceleration(sf::Vector2f(xAcceleration * moveSpeed, yAcceleration * moveSpeed));
}

void Being::jumpWalk(int rotation, float distance) {
	float xMove = std::sin((-1 * rotation - 180) / 57.29578f);
	float yMove = std::cos((-1 * rotation - 180) / -57.29578f);

	setAcceleration(sf::Vector2f(xMove * distance, yMove * distance));
}

void Being::setTextureRect(sf::IntRect newRect) {
	mySprite.setTextureRect(newRect);
}

float Being::getTargetRotation() {
	return targetRotation;
}

void Being::setTargetRotation(float newValue) {
	if (newValue < 0.f) {
		targetRotation = 360.f + newValue;
	}
	else if (newValue >= 360.f) {
		targetRotation = (int)newValue % 360;
	}
	else {
		targetRotation = newValue;
	}

	//targetRotation = newValue;
}

void Being::updateRotation(settings& settings) {
	float rotationChangeValue = 0.f;

	if (targetRotation < mySprite.getRotation()) {
		if (mySprite.getRotation() - targetRotation > 180.f) {
			rotationChangeValue = (360.f - mySprite.getRotation()) + targetRotation;
		}
		else if (mySprite.getRotation() - targetRotation <= 180.f) {
			rotationChangeValue = -1 * (mySprite.getRotation() - targetRotation);
		}
	}
	else if (targetRotation > mySprite.getRotation()) {
		if (targetRotation - mySprite.getRotation() > 180.f) {
			rotationChangeValue = -1 * (mySprite.getRotation() + (360.f - targetRotation));
		}
		else if (targetRotation - mySprite.getRotation() <= 180.f) {
			rotationChangeValue = targetRotation - mySprite.getRotation();
		}
	}
	else {
		// good
	}

	rotationChangeValue = rotationChangeValue / settings.rotationSpeedDivisor / settings.ticksPerSecond;

	rotation += rotationChangeValue;

	if (rotation >= 360.f) {
		rotation = (int)rotation % 360;
	}

	mySprite.setRotation(rotation);
}
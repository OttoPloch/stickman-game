#pragma once
#include "being.h"
#include "settings.h"
#include "asset_manager.h"

class Player : public Being
{
public:
	Player();

	Player(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID);
	
	void createPlayer(sf::Vector2f position, AssetManager& textureManager, settings& settings, std::string beingType, double ID);
};


#pragma once
#include "camera.h"
#include "settings.h"
#include "asset_manager.h"

class Editor
{
public:
	sf::Sprite mySprite;

	Editor();

	Editor(sf::Vector2f position, AssetManager& textureManager);

	void create(sf::Vector2f position, AssetManager& textureManager);

	void move(sf::Vector2f offset, float dt);

	void update(sf::RenderWindow& window, Camera& camera);

	void draw(sf::RenderWindow& window);
};


#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class AssetManager
{
public:
	sf::Texture texture;

	void load(std::string path);
};


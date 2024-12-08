#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Animation
{
public:
	float timeBetweenFrames;

	int frames;

	std::vector<sf::IntRect> frameRects;

	int currentFrame;

	sf::IntRect currentFrameRect;

	Animation();
	
	Animation(int frames, std::vector<sf::IntRect> frameRects, float timeBetweenFrames);

	void create(int frames, std::vector<sf::IntRect> frameRects, float timeBetweenFrames);

	void step();

	void reset();

};


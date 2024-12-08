#pragma once
#include "animation.h"
#include <map>

class AnimationManager
{
public:
	std::map<std::string, Animation> animations;

	std::string currentAnimation;

	float timePassed;

	AnimationManager();

	void addAnimation(std::string name, int frames, std::vector<sf::IntRect> frameRects, float timeBetweenFrames);

	void step();

	void reset();
};


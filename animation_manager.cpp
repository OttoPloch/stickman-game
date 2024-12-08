#include "animation_manager.h"

AnimationManager::AnimationManager() : timePassed(0.f), currentAnimation("none") {}

void AnimationManager::addAnimation(std::string name, int frames, std::vector<sf::IntRect> frameRects, float timeBetweenFrames) {
	animations[name] = Animation(frames, frameRects, timeBetweenFrames);
}

void AnimationManager::step() {
	timePassed = 0.f;

	animations[currentAnimation].step();
}

void AnimationManager::reset() {
	timePassed = 0.f;

	for (auto &i : animations) {
		i.second.reset();
	}
}

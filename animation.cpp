#include "animation.h"

Animation::Animation() : currentFrame(0), frames(0), timeBetweenFrames(0.025f) {}

Animation::Animation(int frames, std::vector<sf::IntRect> frameRects, float timeBetweenFrames) : currentFrame(0), frames(0), timeBetweenFrames(0.025f) {
	create(frames, frameRects, timeBetweenFrames);
}

void Animation::create(int frames, std::vector<sf::IntRect> frameRects, float timeBetweenFrames) {
	this->timeBetweenFrames = timeBetweenFrames;
	
	this->frames = frames;

	this->frameRects = frameRects;
	
	currentFrame = 0;

	currentFrameRect = frameRects[currentFrame];
}

void Animation::step() {
	currentFrame++;

	if (currentFrame > frames - 1) {
		currentFrame = 0;
	}

	currentFrameRect = frameRects[currentFrame];
}

void Animation::reset() {
	currentFrame = 0;

	currentFrameRect = frameRects[currentFrame];
}
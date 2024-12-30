#pragma once
#include <string>
#include <map>
#include <array>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

struct settings {
	int WINDOW_LENGTH, WINDOW_HEIGHT;

	int minZoomLength, minZoomHeight;
	int maxZoomLength, maxZoomHeight;

	// the fraction of the window size
	// that the default camera size is
	float defaultCameraSizeMultiplier;

	float windowHeightToWidth;

	// higher value means
	// the camera moves slower
	// but does not affect length
	// of move
	float cameraMoveDivisor;

	// higher value means
	// the camera zooms slower
	// but does not affect length
	// of zoom
	float cameraZoomDivisor;

	// higher value means
	// beings rotate towards
	// their target rotation slower
	float rotationSpeedDivisor;

	// higher value means
	// the level editor will change
	// the value that sets the rotation
	// of a new item faster when the
	// proper key is pressed. (used for
	// inputs gathered during ticks)
	float levelEditorRotationSpeedTPS;

	// same as one above, except used for
	// inputs gathered during frames.
	float levelEditorRotationSpeedDT;

	// the rate at which the size of papers
	// can be changed when editing. (used for
	// inputs gathered during ticks)
	sf::Vector2f levelEditorPaperSizeChangeSpeedTPS;

	// same as one above, except used for
	// inputs gathered during frames.
	sf::Vector2f levelEditorPaperSizeChangeSpeedDT;

	float zoomSpeed;

	std::string windowName;

	int ticksPerSecond;
	double millisPerTick;

	int paperFriction, woodFriction;

	std::map<std::string, float> moveSpeeds;
	std::map<std::string, int> maxHealths;
	std::map<std::string, std::array<float, 2>> collisionRectSizes;
	std::map<std::string, sf::Vector2f> spriteSizes;

	float cubeJumpDistance;

	// this is the value that the enemy's
	// timer needs to be at to jump.
	float cubeJumpSpeed;

	// [0] = length of sight triangle
	// [1] = width of sight triangle
	// [2] = radius of follow circle (how far away the being can keep track of a target)
	std::map<std::string, std::array<float, 3>> sightLengthsWidthsLimits;

	int levels;
	int levelIndex;

	std::map<std::string, std::string> levelFilePaths;	

	float transitionSpeed;

	float paperEraseCircleRad;
};

void createSettings(settings& settings);

void adjustWindowSettings(settings& settings, int newWindowLength, int newWindowHeight, sf::Vector2f worldSize);
#include "settings.h"

void createSettings(settings& settings) {
	// hard coding the world size is fine since it is always going
	// to load the 1st level first, however it is not ideal.
	adjustWindowSettings(settings, 1280, 720, sf::Vector2f(3840, 2140));

	settings.defaultCameraSizeMultiplier = 0.5f;

	settings.cameraMoveDivisor = 0.2f;

	settings.cameraZoomDivisor = 0.2f;

	settings.rotationSpeedDivisor = 0.2f;

	settings.levelEditorRotationSpeedTPS = 100.f;

	settings.levelEditorRotationSpeedDT = 600.f;

	settings.levelEditorPaperSizeChangeSpeedTPS = sf::Vector2f(150, 200);

	settings.levelEditorPaperSizeChangeSpeedDT = sf::Vector2f(1000, 1333);

	settings.zoomSpeed = 1000.f;

	settings.windowName = "my cute little stickman game";

	settings.ticksPerSecond = 60;
	settings.millisPerTick = 1000.f / settings.ticksPerSecond;

	settings.paperFriction = 1000;

	settings.moveSpeeds = {
		{"player", 250.f},
		{"enemy_ghoul", 100.f},
		// the cube jumps, not walks so this doesn't affect it
		{"enemy_cube", 1.f}
	};

	settings.maxHealths = {
		{"player", 100},
		{"enemy_ghoul", 80},
		{"enemy_cube", 300}
	};

	settings.collisionRectSizes = {
		{"player", {10, 70}},
		{"enemy_ghoul", {60, 60}},
		{"enemy_cube", {90, 90}}
	};

	settings.spriteSizes = {
		{"player", {30, 80}},
		{"enemy_ghoul", {100, 100}},
		{"enemy_cube", {150, 150}}
	};

	settings.cubeJumpDistance = 850.f;
	settings.cubeJumpSpeed = 3.5f;

	settings.sightLengthsWidthsLimits = {
		{"enemy_ghoul", {450.f, 900.f, 650.f}},
		{"enemy_cube", {350.f, 650.f, 800.f}}
	};

	settings.levels = 4;
	settings.levelIndex = 1;

	settings.levelFilePaths = {
		{"player", "player.txt"},
		{"papers", "papers.txt"},
		{"ghouls", "ghouls.txt"},
		{"camera", "camera.txt" },
		{"cubes", "cubes.txt"}
	};

	settings.transitionSpeed = 225.f;

	settings.paperEraseCircleRad = 50.f;
}

void adjustWindowSettings(settings& settings, int newWindowLength, int newWindowHeight, sf::Vector2f worldSize) {
	settings.WINDOW_LENGTH = newWindowLength;
	settings.WINDOW_HEIGHT = newWindowHeight;

	settings.windowHeightToWidth = settings.WINDOW_LENGTH * 1.f / settings.WINDOW_HEIGHT;

	if (settings.windowHeightToWidth > 1.78f) {
		settings.minZoomLength = worldSize.x / 12;

		settings.maxZoomLength = worldSize.x - 1;

		settings.minZoomHeight = settings.minZoomLength / settings.windowHeightToWidth;

		settings.maxZoomHeight = settings.maxZoomLength / settings.windowHeightToWidth;
	}
	else if (settings.windowHeightToWidth < 1.77f) {
		settings.minZoomHeight = worldSize.y / 12;

		settings.maxZoomHeight = worldSize.y - 1;

		settings.minZoomLength = settings.minZoomHeight * settings.windowHeightToWidth;

		settings.maxZoomLength = settings.maxZoomHeight * settings.windowHeightToWidth;
	}
	else {
		settings.minZoomHeight = worldSize.y / 12;

		settings.maxZoomHeight = worldSize.y - 1;
		
		settings.minZoomLength = worldSize.x / 12;
		
		settings.maxZoomLength = worldSize.x - 1;
	}
}
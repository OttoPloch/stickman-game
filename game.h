#pragma once
#include "settings.h"
#include "level.h"
#include "input_manager.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "editing_visuals.h"

class Game
{
public:
	sf::RenderWindow window;

	settings settings;

	sf::Clock clock;

	Level level;

	InputManager inputManager;

	EditingVisuals editingVisuals;

	sf::ContextSettings windowSettings;

	bool paused;

	bool editing;

	bool incrementingLevel;
	bool switchingToEdit;

	void start();

	void run();

	void checkTransition();

	void frameInput(float dt);

	void tickInput();

	void events();

	void tick(float dt);

	void render(float dt);

	void close();

	void levelIncrement();

	void levelEditSwitch();
};


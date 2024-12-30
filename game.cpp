#include "game.h"

void Game::start() {
	createSettings(settings);

	editing = false;

	windowSettings.antialiasingLevel = 8;

	window.create(sf::VideoMode(settings.WINDOW_LENGTH, settings.WINDOW_HEIGHT), settings.windowName, sf::Style::Default, windowSettings);

	level.create(settings, "./levels/level 1/", window, editing);

	inputManager.create();

	editingVisuals.create(level.paperTexCoords, level.textures, level.camera);

	incrementingLevel = false;
	switchingToEdit = false;

	run();
}

void Game::run() {
	double ticksToProcess = 0;
	float timePassed = 0;
	int frames = 0;
	int ticks = 0;

	while (window.isOpen()) {
		float dt = clock.restart().asSeconds();

		frameInput(dt);

		events();

		if (!paused) {
			ticksToProcess += (dt * 1000.f) / settings.millisPerTick;

			while (ticksToProcess >= 1) {
				ticks++;

				tick(dt);

				tickInput();

				ticksToProcess -= 1;
			}
		}

		frames++;

		render(dt);

		checkTransition();

		timePassed += dt;
		if (timePassed >= 1) {
			timePassed = 0;

			std::cout << "ticks: " << ticks << ", frames: " << frames << '\n';

			ticks = 0;
			frames = 0;
		}

		sf::sleep(sf::milliseconds(1));
	}
}

void Game::checkTransition() {
	if (incrementingLevel) {
		if (level.transitioning == false) {
			levelIncrement();
		}
	}

	if (switchingToEdit) {
		if (level.transitioning == false) {
			levelEditSwitch();
		}
	}
}

void Game::frameInput(float dt) {
	inputManager.update();

	if (!paused) {
 		sf::Vector2f moveVector(0.f, 0.f);

		if (inputManager.inputs.w) {
			moveVector.y = -1.f;
		}
		else if (inputManager.inputs.s) {
			moveVector.y = 1.f;
		}

		if (inputManager.inputs.a) {
			moveVector.x = -1.f;
		}
		else if (inputManager.inputs.d) {
			moveVector.x = 1.f;
		}

		if (moveVector.x != 0 || moveVector.y != 0) {
			if (!editing) {
				level.player.walk(moveVector);
			}
			else {
				level.editor.move(sf::Vector2f(moveVector.x * 400, moveVector.y * 400), dt);
			}
		}

		int min[] = { settings.minZoomLength, settings.minZoomHeight };
		int max[] = { settings.maxZoomLength, settings.maxZoomHeight };

		if (inputManager.inputs.q) {
			level.camera.zoom(-1.f, dt, settings.windowHeightToWidth, min, max, settings.zoomSpeed);
		}
		if (inputManager.inputs.e) {
			level.camera.zoom(1.f, dt, settings.windowHeightToWidth, min, max, settings.zoomSpeed);
		}

		if (inputManager.inputs.w || inputManager.inputs.a || inputManager.inputs.s || inputManager.inputs.d) {
			level.camera.pressingAMoveKey();
		}
	}
	else {
		if (inputManager.inputs.z) {
			level.levelEditor.changeRotationValue(-1.f * settings.levelEditorRotationSpeedDT, dt);
		}

		if (inputManager.inputs.c) {
			level.levelEditor.changeRotationValue(settings.levelEditorRotationSpeedDT, dt);
		}

		if (inputManager.inputs.x) {
			level.levelEditor.changePaperDimensions(settings.levelEditorPaperSizeChangeSpeedDT, dt);
		}
	}
}

void Game::tickInput() {
	inputManager.update();

	if (inputManager.inputs.z) {
		level.levelEditor.changeRotationValue(-1.f * settings.levelEditorRotationSpeedTPS, settings.ticksPerSecond);
	}

	if (inputManager.inputs.c) {
		level.levelEditor.changeRotationValue(settings.levelEditorRotationSpeedTPS, settings.ticksPerSecond);
	}

	if (inputManager.inputs.x) {
		level.levelEditor.changePaperDimensions(settings.levelEditorPaperSizeChangeSpeedTPS, settings.ticksPerSecond);
	}
}

void Game::events() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (window.hasFocus()) {
			switch (event.type) {
			case sf::Event::Closed:
				close();
				break;
			case sf::Event::Resized:
				adjustWindowSettings(settings, event.size.width, event.size.height, level.worldSize);
				level.camera.windowResize(sf::Vector2f(event.size.width, event.size.height), settings.defaultCameraSizeMultiplier);
				break;
			case sf::Event::KeyPressed:
				if (window.hasFocus()) {
					if (event.key.code == sf::Keyboard::F1) {
						level.showRects = !level.showRects;
					}
					if (event.key.code == sf::Keyboard::P) {
						level.incrementFocus();
					}
					if (event.key.code == sf::Keyboard::Escape) {
						paused = !paused;
					}
					if (event.key.code == sf::Keyboard::Enter) {
						if (!incrementingLevel && !switchingToEdit) {
							level.startTransition(1);

							incrementingLevel = true;
						}
					}
					if (event.key.code == sf::Keyboard::F3) {
						if (!incrementingLevel && !switchingToEdit) {
							level.startTransition(1);

							switchingToEdit = true;
						}
					}
					if (event.key.code == sf::Keyboard::F4) {
						if (editing) {
							level.levelEditor.switchMode();
						}
					}
					if (event.key.code == sf::Keyboard::R) {
						if (editing) {
							level.levelEditor.incrementTool();
						}
					}
				}

				break;
			case sf::Event::MouseButtonPressed:
				if (window.hasFocus()) { 
					if (editing) {
						if (sf::Mouse::getPosition(window).x < window.getSize().x && sf::Mouse::getPosition(window).x > 0) {
							if (sf::Mouse::getPosition(window).y < window.getSize().y && sf::Mouse::getPosition(window).y > 0) {
								Paper* collidedPaper = level.checkMousePaperCollision(window, settings);
								Being* collidedBeing = level.checkMouseBeingCollision(level.levelEditor.currentEditTool, window);
								
								level.levelEditor.click(window, level.camera, collidedPaper, collidedBeing);

								if (!level.levelEditor.erasing) {
									editingVisuals.addItemToDraw(level.levelEditor.currentEditTool);
								}
								else {
									sf::Vector2f deleteSpritePos = sf::Vector2f(-69.696969f, -69.696969f);
									
									if (level.levelEditor.currentEditTool == "paper") {
										if (collidedPaper != nullptr) {
											deleteSpritePos = collidedPaper->pos;
										}
									}
									else {
										if (collidedBeing != nullptr) {
										deleteSpritePos = collidedBeing->mySprite.getPosition();
										}
									}

									if (deleteSpritePos != sf::Vector2f(-69.696969f, -69.696969f) && level.levelEditor.currentEditTool != "player") {
										editingVisuals.addXToDraw(deleteSpritePos, settings);
									}
								}
							}
						}
					}
				}
			}

		}
	}
}

void Game::tick(float dt) {
	level.update(window, editing);
}

void Game::render(float dt)  {
	window.clear();

	level.drawBackground(window);

	level.draw(window, dt, editing, settings);

	if (editing) {
		editingVisuals.updateAndDraw(window, level.camera, level.levelEditor.currentEditTool, level.levelEditor.getPaperDimensions(), level.levelEditor.getRotationValue(), level.levelEditor.erasing);
	}

	level.updateAndDrawTransitionRect(window, dt);

	window.display();
}

void Game::close() {
	level.levelEditor.updateFiles("./levels/level " + std::to_string(settings.levelIndex) + "/", settings);
	
	window.close();
}

void Game::levelIncrement() {
	paused = false;
	
	if (editing) {
		level.levelEditor.updateFiles("./levels/level " + std::to_string(settings.levelIndex) + "/", settings);
		editingVisuals.clearDraws();
	}
	
	settings.levelIndex++;

	if (settings.levelIndex > settings.levels) {
		settings.levelIndex = 1;
	}

	level = Level();

	level.create(settings, "./levels/level " + std::to_string(settings.levelIndex) + '/', window, editing);

	incrementingLevel = false;
}

void Game::levelEditSwitch() {
	paused = false;
	
	if (editing) {
		level.levelEditor.updateFiles("./levels/level " + std::to_string(settings.levelIndex) + "/", settings);
		editingVisuals.clearDraws();
	}
	
	editing = !editing;

	level = Level();

	level.create(settings, "./levels/level " + std::to_string(settings.levelIndex) + '/', window, editing);

	switchingToEdit = false;
}
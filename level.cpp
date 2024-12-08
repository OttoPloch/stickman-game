#include "level.h"

void Level::create(settings& settings, std::string folderPath, sf::RenderWindow& window, bool editingMode) {
	paperTexture.load("./paper.png");
	tabletopTexture.load("./tabletop.png");
	ghoulTexture.load("./ghoul.png");
	playerTexture.load("./player sprite sheet.png");
	cubeTexture.load("./cube.png");

	ghostPaperTexture.load("./ghost paper.png");
	playerSampleTexture.load("./player.png");

	editorTexture.load("./editor.png");

	textures = {
		{"paper", paperTexture},
		{"tabletop", tabletopTexture},
		{"ghoul", ghoulTexture},
		{"ghost paper", ghostPaperTexture},
		{"player", playerSampleTexture},
		{"cube", cubeTexture}
	};

	paperTexCoords = { sf::Vector2u(0, 0), sf::Vector2u(750, 0), sf::Vector2u(750, 1000), sf::Vector2u(0, 1000) };

	IDValue = 0.f;

	loadLevel(folderPath, settings, editingMode);

	for (int i = 0; i < allEnemies.size(); i++) {
		allBeings.push_back(&allEnemies[i]);
	}

	for (int i = 0; i < papers.size(); i++) {
		for (int j = 0; j < 4; j++) {
			paperVertices.push_back(papers[i].vertices[j]);
		}
	}

	paperStates.texture = &paperTexture.texture;

	worldSize = sf::Vector2f(abs(camera.maxPos.x - camera.minPos.x), abs(camera.maxPos.y - camera.minPos.y));

	std::array<sf::Vector2u, 4> tabletopTexCoords = { sf::Vector2u(0, 0), sf::Vector2u(tabletopTexture.texture.getSize().x, 0), sf::Vector2u(tabletopTexture.texture.getSize().x, tabletopTexture.texture.getSize().y), sf::Vector2u(0, tabletopTexture.texture.getSize().y) };

	tabletop.create(sf::Vector2f(camera.minPos.x, camera.minPos.y), camera.maxPos.x - camera.minPos.x, 0, tabletopTexCoords);

	tabletopVertices = { tabletop.vertices[0], tabletop.vertices[1] , tabletop.vertices[2] , tabletop.vertices[3] };

	tabletopStates.texture = &tabletopTexture.texture;

	mySettings = &settings;

	showRects = false;

	cameraFocusIndex = 0;

	levelEditor.create("paper", settings, folderPath);

	transitionRectOpacity = 255.f;
	transitionRect.setSize(camera.view.getSize());
	transitionRect.setFillColor(sf::Color(0, 0, 0, transitionRectOpacity));

	startTransition(-1);

	if (editingMode) {
		editor.create(sf::Vector2f(camera.maxPos.x / 2.f, camera.maxPos.y / 2.f), editorTexture); 
	}
}

void Level::loadLevel(std::string folderPath, settings& settings, bool editingMode) {
	std::ifstream playerFile(folderPath + settings.levelFilePaths["player"]);
	std::ifstream cameraFile(folderPath + settings.levelFilePaths["camera"]);
	std::ifstream paperFile(folderPath + settings.levelFilePaths["papers"]);
	std::ifstream ghoulFile(folderPath + settings.levelFilePaths["ghouls"]);
	std::ifstream cubeFile(folderPath + settings.levelFilePaths["cubes"]);

	std::array<std::ifstream*, 5> files = {&playerFile, &cameraFile, &paperFile, &ghoulFile, &cubeFile};

	std::ifstream* currentFile = nullptr;
	for (int i = 0; i < 5; i++) {	
		currentFile = files[i];
	
		if (currentFile != nullptr) {
			if (currentFile->is_open()) {
				std::string line;

				while (std::getline(*currentFile, line)) {
					std::string identifier = line.substr(0, 3);

					if (identifier == "plr") {
						player.createPlayer(sf::Vector2f(std::stof(line.substr(4, 5)), std::stof(line.substr(10, 5))), playerTexture, settings, "player", getIDAndIncrement());
						allBeings.push_back(&player);
					}
					else if (identifier == "cam") {
						sf::Vector2f minPos(std::stof(line.substr(4, 5)), std::stof(line.substr(10, 5)));
						sf::Vector2f maxPos(std::stof(line.substr(16, 5)), std::stof(line.substr(22, 5)));

						sf::Sprite* focus;

						if (editingMode) {
							focus = &editor.mySprite;
						}
						else {
							focus = &player.mySprite;
						}

						camera.create(focus, sf::Vector2f(settings.WINDOW_LENGTH * settings.defaultCameraSizeMultiplier, settings.WINDOW_HEIGHT * settings.defaultCameraSizeMultiplier), minPos, maxPos, !editingMode);
					}
					else if (identifier == "ppr") {
						std::array<sf::Vector2u, 4> texCoords;

						if (line.substr(27, 4) == "norm") {
							texCoords = paperTexCoords;
						}

						papers.push_back(Paper(sf::Vector2f(std::stof(line.substr(4, 5)), std::stof(line.substr(10, 5))), std::stof(line.substr(16, 5)), std::stof(line.substr(22, 4)), texCoords));
					}
					else if (identifier == "gul") {
						allEnemies.push_back(Enemy(sf::Vector2f(std::stof(line.substr(4, 5)), std::stof(line.substr(10, 5))), ghoulTexture, settings, "enemy_ghoul", getIDAndIncrement()));
					}
					else if (identifier == "cub") {
						allEnemies.push_back(Enemy(sf::Vector2f(std::stof(line.substr(4, 5)), std::stof(line.substr(10, 5))), cubeTexture, settings, "enemy_cube", getIDAndIncrement()));
					}
				}
			}
			else {
				std::cout << "ERROR couldn't open file for level loading\n";
			}

			currentFile->close();
		}
	}
}

double Level::getIDAndIncrement() {
	double val = IDValue;

	IDValue += 0.1f;

	return val;
}

void Level::incrementFocus() {
	cameraFocusIndex++;

	if (cameraFocusIndex >= allBeings.size()) {
		cameraFocusIndex = 0;
	}

	camera.setFocus(&allBeings[cameraFocusIndex]->mySprite);
}

void Level::drawBackground(sf::RenderWindow& window) {
	window.draw(&tabletopVertices[0], tabletopVertices.size(), sf::Quads, tabletopStates);
}

void Level::draw(sf::RenderWindow& window, float dt, bool& editing) {
	// called update because this affects rendering, not part of gameplay
	camera.update(window, dt, mySettings);

	window.draw(&paperVertices[0], paperVertices.size(), sf::Quads, paperStates);

	player.draw(window, showRects);
	
	for (int i = 0; i < allEnemies.size(); i++) {
		allEnemies[i].draw(window, showRects);
	}

	if (showRects) {
		for (int i = 0; i < allEnemies.size(); i++) {
			allEnemies[i].drawRects(window);
		}
	}

	if (editing) {
		editor.draw(window);
	}
}

void Level::updateAndDrawTransitionRect(sf::RenderWindow& window, float dt) {
	if (transitioning) {
		transitionStep(dt);

		window.draw(transitionRect);
	}

	if (transitionRectOpacity == 255) {
		window.draw(transitionRect);
	}
}

void Level::update(sf::RenderWindow& window, bool& editing) {
	if (!editing) {
		collisions();

		player.update(*mySettings);

		for (int i = 0; i < allEnemies.size(); i++) {
			allEnemies[i].updateEnemy(*mySettings, window);
		}
	}
}

void Level::collisions() {
	for (int i = 0; i < allEnemies.size(); i++) {
		std::vector<Being*> collidedBeings;

		for (int j = 0; j < allBeings.size(); j++) {
			if (allBeings[j]->getID() == allEnemies[i].getID()) {
				continue;
			} 

			if (triangleCollide(allEnemies[i].sightTriangle, allBeings[j]->getCollisionRect())) {
				collidedBeings.push_back(allBeings[j]);
			}
		}

		allEnemies[i].collidedBeings = collidedBeings;
	}
}

void Level::startTransition(int direction) {
	transitioning = true;

	transitionDirection = direction;
}

void Level::transitionStep(float dt) {
	transitionRectOpacity += transitionDirection * mySettings->transitionSpeed * dt;

	if (transitionRectOpacity < 0.f) {
		transitionRectOpacity = 0.f;
		transitioning = false;
	}
	else if (transitionRectOpacity > 255.f) {
		transitionRectOpacity = 255.f;
		transitioning = false;
	}

	transitionRect.setSize(camera.view.getSize());
	transitionRect.setPosition(camera.getTopLeft());
	transitionRect.setFillColor(sf::Color(0, 0, 0, transitionRectOpacity));
}

Paper* Level::checkMousePaperCollision(sf::RenderWindow& window) {
	int mouseX = (int)camera.getTopLeft().x + ((float)sf::Mouse::getPosition(window).x / window.getSize().x * 1.f) * camera.view.getSize().x;
	int mouseY = (int)camera.getTopLeft().y + ((float)sf::Mouse::getPosition(window).y / window.getSize().y * 1.f) * camera.view.getSize().y;

	for (int i = 0; i < papers.size(); i++) {
		bool mouseCollides = false;
		
		if (mouseCollides) {
			return &papers[i];
		}
	}

	return nullptr;
}

Being* Level::checkMouseBeingCollision(std::string beingTypeSearch, sf::RenderWindow& window) {
	int mouseX = (int)camera.getTopLeft().x + ((float)sf::Mouse::getPosition(window).x / window.getSize().x * 1.f) * camera.view.getSize().x;
	int mouseY = (int)camera.getTopLeft().y + ((float)sf::Mouse::getPosition(window).y / window.getSize().y * 1.f) * camera.view.getSize().y;

	for (int i = 0; i < allBeings.size(); i++) {
		if (allBeings[i]->beingType != beingTypeSearch) {
			continue;
		}

		bool mouseCollides = false;
		
		float beingILeft = allBeings[i]->getCollisionRect().getPosition().x - allBeings[i]->getCollisionRect().getOrigin().x;
		float beingITop = allBeings[i]->getCollisionRect().getPosition().y - allBeings[i]->getCollisionRect().getOrigin().y;

		if (mouseX - beingILeft >= 0.f && mouseX - beingILeft <= allBeings[i]->getCollisionRect().getLocalBounds().width) {
			if (mouseY - beingITop >= 0.f && mouseY - beingITop <= allBeings[i]->getCollisionRect().getLocalBounds().height) {
				return allBeings[i];
			}
		}
	}

	return nullptr;
}


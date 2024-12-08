#include "editing_visuals.h"

EditingVisuals::EditingVisuals() {}

EditingVisuals::EditingVisuals(std::array<sf::Vector2u, 4> paperTexCoords, std::map<std::string, AssetManager> textures) {
	create(paperTexCoords, textures);
}

void EditingVisuals::create(std::array<sf::Vector2u, 4> paperTexCoords, std::map<std::string, AssetManager> textures) {
	this->paperTexCoords = paperTexCoords;
	
	paperVertices[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(paperTexCoords[0].x, paperTexCoords[0].y));
	paperVertices[1] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(paperTexCoords[1].x, paperTexCoords[1].y));
	paperVertices[2] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(paperTexCoords[2].x, paperTexCoords[2].y));
	paperVertices[3] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(paperTexCoords[3].x, paperTexCoords[3].y));

	itemTextures = textures;

	paperStates.texture = &itemTextures["paper"].texture;

	ghostPaperStates.texture = &itemTextures["ghost paper"].texture;

	cubeSprite.setTexture(itemTextures["cube"].texture);
	cubeSprite.setOrigin(cubeSprite.getLocalBounds().width / 2.f, cubeSprite.getLocalBounds().height / 2.f);

	ghoulSprite.setTexture(itemTextures["ghoul"].texture);
	ghoulSprite.setOrigin(ghoulSprite.getLocalBounds().width / 2.f, ghoulSprite.getLocalBounds().height / 2.f);

	playerSprite.setTexture(itemTextures["player"].texture);
	playerSprite.setOrigin(playerSprite.getLocalBounds().width / 2.f, playerSprite.getLocalBounds().height / 2.f);

	ghostPlayerSprite.setTexture(itemTextures["player"].texture);
	ghostPlayerSprite.setColor(sf::Color(255, 255, 255, 180));
	ghostPlayerSprite.setOrigin(ghostPlayerSprite.getLocalBounds().width / 2.f, ghostPlayerSprite.getLocalBounds().height / 2.f);
	ghostPlayerSprite.setPosition(-1, -1);

	pencilTexture.loadFromFile("./pencil.png");
	XTexture.loadFromFile("./x mark.png");

	pencilSprite.setTexture(pencilTexture);
	pencilSprite.setOrigin(pencilSprite.getLocalBounds().width / 2.f, pencilSprite.getLocalBounds().height / 2.f);
}

void EditingVisuals::clearDraws() {
	ghostPaperVertices.clear();
	ghostPlayerSprite.setPosition(-1, -1);
	ghostGhouls.clear();
	ghostCubes.clear();
	xSprites.clear();
}

void EditingVisuals::addItemToDraw(std::string itemType) {
	if (itemType == "paper") {
		for (int i = 0; i < 4; i++) {
			ghostPaperVertices.push_back(paperVertices[i]);
		}
	}
	else if (itemType == "enemy_ghoul") {
		ghostGhouls.push_back(sf::Sprite(*ghoulSprite.getTexture()));

		sf::Sprite* currentGhoul = &ghostGhouls[ghostGhouls.size() - 1];

		currentGhoul->setOrigin(currentGhoul->getLocalBounds().width / 2.f, currentGhoul->getLocalBounds().height / 2.f);
		currentGhoul->setPosition(worldMousePos);
		currentGhoul->setColor(sf::Color(255, 255, 255, 180));
	}
	else if (itemType == "player") {
		ghostPlayerSprite.setPosition(worldMousePos);
	}
	else if (itemType == "enemy_cube") {
		ghostCubes.push_back(sf::Sprite(*cubeSprite.getTexture()));

		sf::Sprite* currentCube = &ghostCubes[ghostCubes.size() - 1];

		currentCube->setOrigin(currentCube->getLocalBounds().width / 2.f, currentCube->getLocalBounds().height / 2.f);
		currentCube->setPosition(worldMousePos);
		currentCube->setColor(sf::Color(255, 255, 255, 180));
	}
}

void EditingVisuals::addXToDraw(sf::Vector2f pos) {
	xSprites.push_back(sf::Sprite(XTexture));

	sf::Sprite* currentX = &xSprites[xSprites.size() - 1];

	currentX->setOrigin(currentX->getLocalBounds().width / 2.f, currentX->getLocalBounds().height / 2.f);
	currentX->setPosition(pos);
	currentX->setColor(sf::Color(255, 255, 255, 180));
}

void EditingVisuals::updateAndDraw(sf::RenderWindow& window, Camera& camera, std::string currentEditTool, sf::Vector2i paperDimensions, float paperRotation, bool erasing) {
	worldMousePos.x = (int)camera.getTopLeft().x + ((float)sf::Mouse::getPosition(window).x / window.getSize().x * 1.f) * camera.view.getSize().x;
	worldMousePos.y = (int)camera.getTopLeft().y + ((float)sf::Mouse::getPosition(window).y / window.getSize().y * 1.f) * camera.view.getSize().y;

	if (ghostPaperVertices.size() > 0) {
		window.draw(&ghostPaperVertices[0], ghostPaperVertices.size(), sf::Quads, ghostPaperStates);
	}

	if (ghostGhouls.size() > 0) {
		for (int i = 0; i < ghostGhouls.size(); i++) {
			window.draw(ghostGhouls[i]);
		}
	}

	if (ghostPlayerSprite.getPosition() != sf::Vector2f(-1, -1)) {
		window.draw(ghostPlayerSprite);
	}

	if (ghostCubes.size() > 0) {
		for (int i = 0; i < ghostCubes.size(); i++) {
			window.draw(ghostCubes[i]);
		}
	}

	if (xSprites.size() > 0) {
		for (int i = 0; i < xSprites.size(); i++) {
			window.draw(xSprites[i]);
		}
	}

	if (currentEditTool == "paper") {
		paperVertices[0].position = worldMousePos;
		paperVertices[1].position = getPosFromRaycast(worldMousePos, paperDimensions.x, paperRotation + 90);
		paperVertices[2].position = getPosFromRaycast(paperVertices[1].position, paperDimensions.y, paperRotation + 180.f);
		paperVertices[3].position = getPosFromRaycast(paperVertices[2].position, paperDimensions.x, paperRotation + 270.f);

		window.draw(&paperVertices[0], paperVertices.size(), sf::Quads, paperStates);
	}
	else if (currentEditTool == "enemy_ghoul") {
		ghoulSprite.setPosition(worldMousePos);

		window.draw(ghoulSprite);
	}
	else if (currentEditTool == "player") {
		playerSprite.setPosition(worldMousePos);

		window.draw(playerSprite);
	}
	else if (currentEditTool == "enemy_cube") {
		cubeSprite.setPosition(worldMousePos);

		window.draw(cubeSprite);
	}

	if (erasing) {
		pencilSprite.setRotation(180.f);
	}
	else {
		pencilSprite.setRotation(0.f);
	}

	pencilSprite.setPosition(worldMousePos);

	window.draw(pencilSprite);
}
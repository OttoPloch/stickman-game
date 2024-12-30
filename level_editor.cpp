#include "level_editor.h"

LevelEditor::LevelEditor() : rotation(0.f), erasing(false) {}

LevelEditor::LevelEditor(std::string editTool, settings& settings, std::string folderPath) {
	create(editTool, settings, folderPath);
}

void LevelEditor::create(std::string editTool, settings& settings, std::string folderPath) {
	currentEditTool = editTool;

	rotation = 0.f;

	paperDimensions = sf::Vector2i(300, 400);

	erasing = false;

	playerInputFile = folderPath + settings.levelFilePaths["player"];
	papersInputFile = folderPath + settings.levelFilePaths["papers"];
	ghoulsInputFile = folderPath + settings.levelFilePaths["ghouls"];
	cubesInputFile = folderPath + settings.levelFilePaths["cubes"];

	playerTemporaryFile = folderPath + "player_temp.txt";
	papersTemporaryFile = folderPath + "papers_temp.txt";
	ghoulsTemporaryFile = folderPath + "ghouls_temp.txt";
	cubesTemporaryFile = folderPath + "cubes_temp.txt";

	inputFileStrs = { playerInputFile, papersInputFile, ghoulsInputFile, cubesInputFile };
	temporaryFileStrs = { playerTemporaryFile, papersTemporaryFile, ghoulsTemporaryFile, cubesTemporaryFile };
	outputFileStrs = { playerInputFile, papersInputFile, ghoulsInputFile, cubesInputFile };

	pathsForFiles = {
		{"player", playerTemporaryFile},
		{"paper", papersTemporaryFile},
		{"enemy_ghoul", ghoulsTemporaryFile},
		{"enemy_cube", cubesTemporaryFile}
	};

	makeFileCopies(settings, folderPath);

	papersToErase.resize(0);
	ghoulsToErase.resize(0);
	cubesToErase.resize(0);
}

void LevelEditor::makeFileCopies(settings& settings, std::string folderPath) {
	std::string line;

	std::string currentInputFile = "none";
	std::string currentTemporaryFile = "none";
	for (int i = 0; i < 4; i++) {
		currentInputFile = inputFileStrs[i];
		currentTemporaryFile = temporaryFileStrs[i];

		std::ifstream currentInFile(currentInputFile);
		if (!currentInFile) {
			std::cout << "ERROR loading file with path " << currentInputFile << '\n';
		}

		std::ofstream currentTempFile(currentTemporaryFile);
		if (!currentTempFile) {
			std::cout << "ERROR loading file with path " << currentTemporaryFile << '\n';
		}

		while (std::getline(currentInFile, line)) {
			currentTempFile << line << '\n';
		}

		currentInFile.close();
		currentTempFile.close();
	}
}

void LevelEditor::setPaperDimensions(sf::Vector2i newValue) {
	paperDimensions = newValue;
}

void LevelEditor::changePaperDimensions(sf::Vector2f value, float dt) {
	paperDimensions.x += value.x * dt;
	paperDimensions.y += value.y * dt;
}

void LevelEditor::changePaperDimensions(sf::Vector2f value, int ticksPerSecond) {
	paperDimensions.x += value.x / ticksPerSecond;
	paperDimensions.y += value.y / ticksPerSecond;
}

sf::Vector2i LevelEditor::getPaperDimensions() {
	return paperDimensions;
}

float LevelEditor::getRotationValue() {
	return rotation;
}

void LevelEditor::setRotationValue(int newValue) {
	rotation = newValue;

	rotation = (int)rotation % 360;
}

void LevelEditor::changeRotationValue(int value, float dt) {
	rotation += value * dt;

	rotation = (int)rotation % 360;
}

void LevelEditor::changeRotationValue(int value, int ticksPerSecond) {
	rotation += value / ticksPerSecond;

	rotation = (int)rotation % 360;
}

void LevelEditor::switchMode() {
	erasing = !erasing;
}

void LevelEditor::switchTool(std::string newTool) {
	currentEditTool = newTool;
}

void LevelEditor::incrementTool() {
	if (currentEditTool == "player") {
		currentEditTool = "paper";
	}
	else if (currentEditTool == "paper") {
		currentEditTool = "enemy_ghoul";
	}
	else if (currentEditTool == "enemy_ghoul") {
		currentEditTool = "enemy_cube";
	}
	else if (currentEditTool == "enemy_cube") {
		currentEditTool = "player";
	}
}

void LevelEditor::click(sf::RenderWindow& window, Camera& camera, Paper* collidedPaper, Being* collidedBeing) {
	if (erasing) {
		if (currentEditTool == "paper" && collidedPaper != nullptr) {
			erasePaper(collidedPaper);
		}
		else if (currentEditTool.substr(0, 6) == "enemy_" && collidedBeing != nullptr) {
			eraseBeing(collidedBeing);
		}
	}
	else {
		addItem(window, camera);
	}
}

void LevelEditor::addItem(sf::RenderWindow& window, Camera& camera) {
	std::ofstream outputFile;
	
	if (currentEditTool == "player") {
		outputFile.open(pathsForFiles[currentEditTool], std::ios::trunc);
	}
	else {
		outputFile.open(pathsForFiles[currentEditTool], std::ios::app);
	}

	if (!outputFile) {
		std::cout << "ERROR loading output file at path " << pathsForFiles[currentEditTool] << '\n';
	}

	std::string additionString;

	int mouseX = (int)camera.getTopLeft().x + ((float)sf::Mouse::getPosition(window).x / window.getSize().x * 1.f) * camera.view.getSize().x;
	int mouseY = (int)camera.getTopLeft().y + ((float)sf::Mouse::getPosition(window).y / window.getSize().y * 1.f) * camera.view.getSize().y;

	std::string mouseXStr = std::to_string(mouseX);
	std::string mouseYStr = std::to_string(mouseY);

	if (mouseX < 0) {
		mouseXStr.erase(mouseXStr.begin());

		while (mouseXStr.size() < 4) {
			mouseXStr = '0' + mouseXStr;
		}

		mouseXStr = '-' + mouseXStr;
	}
	else {
		while (mouseXStr.size() < 5) {
			mouseXStr = '0' + mouseXStr;
		}
	}

	if (mouseY < 0) {
		mouseYStr.erase(mouseYStr.begin());

		while (mouseYStr.size() < 4) {
			mouseYStr = '0' + mouseYStr;
		}

		mouseYStr = '-' + mouseYStr;
	}
	else {
		while (mouseYStr.size() < 5) {
			mouseYStr = '0' + mouseYStr;
		}
	}

	if (currentEditTool == "player") {
		additionString = "plr " + mouseXStr + " " + mouseYStr;
	}
	else if (currentEditTool == "paper") {
		std::string rotationString = std::to_string((int)rotation);

		if (rotation < 0.f) {
			rotationString.erase(rotationString.begin());

			while (rotationString.size() < 3) {
				rotationString = '0' + rotationString;
			}

			rotationString = '-' + rotationString;
		}
		else {
			while (rotationString.size() < 4) {
				rotationString = '0' + rotationString;
			}
		}

		std::string sizeString = std::to_string(paperDimensions.x);

		while (sizeString.size() < 5) {
			sizeString = '0' + sizeString;
		}

		additionString = "ppr " + mouseXStr + " " + mouseYStr + " " + sizeString + " " + rotationString + " norm";
	}
	else if (currentEditTool == "enemy_ghoul") {
		additionString = "gul " + mouseXStr + " " + mouseYStr;
	}
	else if (currentEditTool == "enemy_cube") {
		additionString = "cub " + mouseXStr + " " + mouseYStr;
	}

	outputFile << additionString << '\n';

	outputFile.close();
}

void LevelEditor::erasePaper(Paper* paper) {
	std::string deletionString = "ppr ";

	std::string paperX = std::to_string(abs(int(paper->pos.x)));

	if (paper->pos.x < 0.f) {
		deletionString += '-';

		while (paperX.size() < 4) {
			paperX = '0' + paperX;
		}
	}
	else {
		while (paperX.size() < 5) {
			paperX = '0' + paperX;
		}
	}

	deletionString += paperX + ' ';

	std::string paperY = std::to_string(abs(int(paper->pos.y)));

	if (paper->pos.y < 0.f) {
		deletionString += '-';

		while (paperY.size() < 4) {
			paperY = '0' + paperY;
		}
	}
	else {
		while (paperY.size() < 5) {
			paperY = '0' + paperY;
		}
	}

	deletionString += paperY + ' ';

	std::string paperWidth = std::to_string(abs(int(paper->width)));

	while (paperWidth.size() < 5) {
		paperWidth = '0' + paperWidth;
	}

	deletionString += paperWidth + ' ';

	std::string paperRotation = std::to_string(abs(int(paper->rotation)));

	if (paper->rotation < 0.f) {
		deletionString += '-';

		while (paperRotation.size() < 3) {
			paperRotation = '0' + paperRotation;
		}
	}
	else {
		while (paperRotation.size() < 4) {
			paperRotation = '0' + paperRotation;
		}
	}

	deletionString += paperRotation + ' ';

	// this doesn't allow for different tex coords, but i also
	// haven't implemented the option for that so idk.
	deletionString += "norm";

	papersToErase.push_back(deletionString);
}

void LevelEditor::eraseBeing(Being* theBeing) {
	std::string deletionString;
	
	std::vector<std::string>* eraseVector = nullptr;

	if (theBeing->beingType == "enemy_ghoul") {
		deletionString = "gul ";
		eraseVector = &ghoulsToErase;
	}
	else if (theBeing->beingType == "enemy_cube") {
		deletionString = "cub ";
		eraseVector = &cubesToErase;
	}


	std::string beingX = std::to_string(abs(int(theBeing->mySprite.getPosition().x)));

	if (theBeing->mySprite.getPosition().x < 0.f) {
		deletionString += '-';

		while (beingX.size() < 4) {
			beingX = '0' + beingX;
		}
	}
	else {
		while (beingX.size() < 5) {
			beingX = '0' + beingX;
		}
	}

	deletionString += beingX + ' ';

	std::string beingY = std::to_string(abs(int(theBeing->mySprite.getPosition().y)));

	if (theBeing->mySprite.getPosition().y < 0.f) {
		deletionString += '-';

		while (beingY.size() < 4) {
			beingY = '0' + beingY;
		}
	}
	else {
		while (beingY.size() < 5) {
			beingY = '0' + beingY;
		}
	}

	deletionString += beingY;

	if (eraseVector != nullptr) {
		eraseVector->push_back(deletionString);
	}
	else {
		std::cout << "ERROR: eraseVector is nullptr. Level Editor\n";
	}
}

void LevelEditor::updateFiles(std::string folderPath, settings& settings) {	
	std::string line;

	std::string currentOutputFile;
	std::string currentTemporaryFile;
	for (int i = 0; i < 4; i++) {
		currentOutputFile = outputFileStrs[i];
		currentTemporaryFile = temporaryFileStrs[i];

		std::ofstream currentOutFile(currentOutputFile);
		if (!currentOutFile) {
			std::cout << "ERROR loading file with path " << currentOutputFile << '\n';
		}

		std::ifstream currentTempFile(currentTemporaryFile);
		if (!currentTempFile) {
			std::cout << "ERROR loading file with path " << currentTemporaryFile << '\n';
		}

		while (std::getline(currentTempFile, line)) {
			bool toDelete = false;

			switch (i) {
			case 1:
				if (papersToErase.size() > 0) {
					if (std::find(papersToErase.begin(), papersToErase.end(), line) != papersToErase.end()) {
						toDelete = true;
						papersToErase.erase(std::find(papersToErase.begin(), papersToErase.end(), line));
					}
				}
				break;
			case 2:
				if (ghoulsToErase.size() > 0) {
					if (std::find(ghoulsToErase.begin(), ghoulsToErase.end(), line) != ghoulsToErase.end()) {
						toDelete = true;
						ghoulsToErase.erase(std::find(ghoulsToErase.begin(), ghoulsToErase.end(), line));
					}
				}
				break;
			case 3:
				if (cubesToErase.size() > 0) {
					if (std::find(cubesToErase.begin(), cubesToErase.end(), line) != cubesToErase.end()) {
						toDelete = true;
						cubesToErase.erase(std::find(cubesToErase.begin(), cubesToErase.end(), line));
					}
				}
				break;
			default:
				break;
			}
			
			if (!toDelete) {
				currentOutFile << line << '\n';
			}
		}

		currentOutFile.close();
		currentTempFile.close();
	}
}
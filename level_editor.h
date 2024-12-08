#pragma once
#include <SFML/Graphics.hpp>
#include "settings.h"
#include <fstream>
#include <iostream>
#include <map>
#include "camera.h"
#include <vector>
#include "being.h"
#include "paper.h"
#include <array>
#include <algorithm>

class LevelEditor
{
public:
	std::string currentEditTool;

	bool erasing;

	LevelEditor();

	LevelEditor(std::string editTool, settings& settings, std::string folderPath);

	void create(std::string editTool, settings& settings, std::string folderPath);

	void makeFileCopies(settings& settings, std::string folderPath);

	sf::Vector2i getPaperDimensions();

	void setPaperDimensions(sf::Vector2i newSize);

	void changePaperDimensions(sf::Vector2f value, float dt);

	void changePaperDimensions(sf::Vector2f value, int ticksPerSecond);

	float getRotationValue();

	void setRotationValue(int newValue);

	void changeRotationValue(int value, float dt);

	void changeRotationValue(int value, int ticksPerSecond);

	void switchMode();

	void switchTool(std::string newTool);

	void incrementTool();

	void click(sf::RenderWindow& window, Camera& camera, Paper* collidedPaper, Being* collidedBeing);

	void addItem(sf::RenderWindow& window, Camera& camera);

	void erasePaper(Paper* paper);

	void eraseBeing(Being* theBeing);

	// called when closing the program.
	// Updates the original text files.
	void updateFiles(std::string folderPath, settings& settings);
private:
	std::string playerInputFile;
	std::string playerTemporaryFile;

	std::string papersInputFile;
	std::string papersTemporaryFile;

	std::string ghoulsInputFile;
	std::string ghoulsTemporaryFile;

	std::string cubesInputFile;
	std::string cubesTemporaryFile;

	std::array<std::string, 4> inputFileStrs;
	std::array<std::string, 4> temporaryFileStrs;
	std::array<std::string, 4> outputFileStrs;

	std::map<std::string, std::string> pathsForFiles;

	std::vector<std::string> papersToErase;
	std::vector<std::string> ghoulsToErase;
	std::vector<std::string> cubesToErase;

	float rotation;
	sf::Vector2i paperDimensions;
};


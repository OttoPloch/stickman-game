#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "asset_manager.h"
#include "paper.h"
#include "player.h"
#include "enemy.h"
#include "being.h"
#include "settings.h"
#include "camera.h"
#include "triangle_collide.h"
#include <fstream>
#include <array>
#include "level_editor.h"
#include <map>
#include "editor.h"

class Level
{
public:
	AssetManager paperTexture;
	AssetManager tabletopTexture;
	AssetManager ghoulTexture;
	AssetManager playerTexture;
	AssetManager cubeTexture;

	AssetManager ghostPaperTexture;
	AssetManager ghostGhoulTexture;
	AssetManager playerSampleTexture;
	AssetManager ghostPlayerSampleTexture;

	AssetManager editorTexture;

	std::map<std::string, AssetManager> textures;

	settings* mySettings;

	std::array<sf::Vector2u, 4> paperTexCoords;

	std::vector<Paper> papers;
	std::vector<sf::Vertex> paperVertices;

	sf::RenderStates paperStates;
	
	BackgroundObject tabletop;
	std::array<sf::Vertex, 4> tabletopVertices;

	sf::RenderStates tabletopStates;

	Player player;

	Camera camera;

	LevelEditor levelEditor;

	Editor editor;

	bool showRects;

	int cameraFocusIndex;

	sf::RectangleShape transitionRect;

	float transitionRectOpacity;

	bool transitioning;

	int transitionDirection;

	sf::Vector2f worldSize;

	void create(settings& settings, std::string folderPath, sf::RenderWindow& window, bool editingMode);

	void loadLevel(std::string folderPath, settings& settings, bool editingMode);

	double getIDAndIncrement();

	void incrementFocus();

	void drawBackground(sf::RenderWindow& window);

	void draw(sf::RenderWindow& window, float dt, bool& editing);

	void updateAndDrawTransitionRect(sf::RenderWindow& window, float dt);

	void update(sf::RenderWindow& window, bool& editing);

	void collisions();

	void startTransition(int direction);

	void transitionStep(float dt);

	Paper* checkMousePaperCollision(sf::RenderWindow& window);

	Being* checkMouseBeingCollision(std::string beingTypeSearch, sf::RenderWindow& window);
private:
	std::vector<Being*> allBeings;
	std::vector<Enemy> allEnemies;

	double IDValue;
};


#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include "get_pos_from_raycast.h"
#include "asset_manager.h"
#include "camera.h"
#include <map>
#include <vector>
#include "camera.h"

class EditingVisuals
{
public:
	std::array<sf::Vertex, 4> paperVertices;

	std::vector<sf::Vertex> ghostPaperVertices;
	
	sf::RenderStates paperStates;
	sf::RenderStates ghostPaperStates;

	std::array<sf::Vector2u, 4> paperTexCoords;

	sf::Sprite cubeSprite;

	sf::Sprite ghoulSprite;

	sf::Sprite ghostPlayerSprite;

	sf::Sprite playerSprite;

	std::map<std::string, AssetManager> itemTextures;

	std::vector<sf::Sprite> ghostGhouls;

	std::vector<sf::Sprite> ghostCubes;

	sf::Texture pencilTexture;
	sf::Texture XTexture;

	sf::Sprite pencilSprite;

	std::vector<sf::Sprite> xSprites;

	EditingVisuals();

	EditingVisuals(std::array<sf::Vector2u, 4> paperTexCoords, std::map<std::string, AssetManager> textures, Camera& camera);

	void create(std::array<sf::Vector2u, 4> paperTexCoords, std::map<std::string, AssetManager> textures, Camera& camera);

	void clearDraws();

	void addItemToDraw(std::string itemType);

	void addXToDraw(sf::Vector2f pos, settings& settings);

	void updateAndDraw(sf::RenderWindow& window, Camera& camera, std::string currentEditTool, sf::Vector2i paperDimensions, float paperRotation, bool erasing);
private:
	sf::Vector2f worldMousePos;

	std::array<sf::Vector2f, 2> cameraBounds;

	sf::RectangleShape levelSizeRect;
};


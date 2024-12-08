#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "settings.h"

class Camera
{
public:
	sf::View view;

	sf::Sprite* focus;

	sf::Vector2f minPos;
	sf::Vector2f maxPos;

	bool moveRestricted;

	Camera();

	Camera(sf::Sprite* focus, sf::Vector2f dimensions, sf::Vector2f minPos, sf::Vector2f maxPos, bool moveRestricted);

	void create(sf::Sprite* focus, sf::Vector2f dimensions, sf::Vector2f minPos, sf::Vector2f maxPos, bool moveRestricted);

	void pressingAMoveKey();

	void snapToFocus();

	void centerOnFocus(float cameraMoveDivisor);

	void setFocus(sf::Sprite* newFocus);

	void updateZoom(float dt, float windowHeightToWidth, float cameraZoomDivisor);

	void zoom(float direction, float dt, float windowHeightToWidth, int minZoomSizes[2], int maxZoomSizes[2], float zoomSpeed);

	void windowResize(sf::Vector2f newDimensions, float defaultCameraSizeMultiplier);

	void setDimensions(sf::Vector2f newDimensions, float defaultCameraSizeMultiplier);

	void changeAcceleration(sf::Vector2f amount, bool settingZoomVar);

	void multAcceleration(sf::Vector2f amount, bool settingZoomVar);

	void setAcceleration(sf::Vector2f amount, bool settingZoomVar);

	sf::Vector2f getAcceleration(bool zoomVar);

	sf::Vector2f getTopLeft();

	void update(sf::RenderWindow& window, float dt, settings* settings);
private:
	sf::Vector2f velocity, acceleration;
	sf::Vector2f zoomVel, zoomAcc;
	sf::Vector2f targetSize;

	// sets to true in zoom(), sets to
	// false in update(). If false, then
	// updateZoom() will snap the zoom when
	// the difference between the view size
	// and target size is minimal.
	bool pressingZoomKey;

	// sets to true in game.cpp in input(),
	// sets to false in update(). If false,
	// then centerOnFocus() will snap the
	// movement when the difference between
	// the view center and focus sprite center
	// is minimal.
	bool pressingMoveKey;
};


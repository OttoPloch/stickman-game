#include "camera.h"

Camera::Camera() : focus(nullptr), pressingZoomKey(false), pressingMoveKey(false), moveRestricted(true) {}

Camera::Camera(sf::Sprite* focus, sf::Vector2f dimensions, sf::Vector2f minPos, sf::Vector2f maxPos, bool moveRestricted) {
	create(focus, dimensions, minPos, maxPos, moveRestricted);
}

void Camera::create(sf::Sprite* focus, sf::Vector2f dimensions, sf::Vector2f minPos, sf::Vector2f maxPos, bool moveRestricted) {
	this->focus = focus;

	view.setSize(dimensions);
	snapToFocus();

	targetSize.x = view.getSize().x;
	targetSize.y = view.getSize().y;

	velocity.x = 0.f;
	velocity.y = 0.f;

	acceleration.x = 0.f;
	acceleration.y = 0.f;

	zoomVel.x = 0.f;
	zoomVel.y = 0.f;

	zoomAcc.x = 0.f;
	zoomAcc.y = 0.f;

	this->minPos = minPos;
	this->maxPos = maxPos;

	pressingZoomKey = false;

	this->moveRestricted = moveRestricted;
}

void Camera::pressingAMoveKey() {
	pressingMoveKey = true;
}

void Camera::snapToFocus() {
	view.setCenter(focus->getPosition());

	//std::cout << focus->getOrigin().x << ", " << focus->getOrigin().y << '\n';
}

void Camera::centerOnFocus(float cameraMoveDivisor) {
	float xDiff = focus->getPosition().x - view.getCenter().x;
	float yDiff = focus->getPosition().y - view.getCenter().y;

	if (abs(xDiff) >= cameraMoveDivisor / 100 || abs(yDiff) >= cameraMoveDivisor / 100 || pressingMoveKey) {
		setAcceleration(sf::Vector2f(xDiff / cameraMoveDivisor, yDiff / cameraMoveDivisor), false);
	}
	else {
		setAcceleration(sf::Vector2f(xDiff, yDiff), false);
	}

}

void Camera::setFocus(sf::Sprite* newFocus) {
	focus = newFocus;
}

void Camera::updateZoom(float dt, float windowHeightToWidth, float cameraZoomDivisor) {
	if (abs(targetSize.x - view.getSize().x) >= cameraZoomDivisor / 100 || abs(targetSize.y - view.getSize().y) >= cameraZoomDivisor / 100 || pressingZoomKey) {
		sf::Vector2f accVector;

		accVector.x = ((targetSize.x - view.getSize().x) / cameraZoomDivisor) * dt;
		accVector.y = ((targetSize.y - view.getSize().y) / cameraZoomDivisor) * dt;

		setAcceleration(accVector, true);
	}
	else {
		setAcceleration(sf::Vector2f(targetSize.x - view.getSize().x, targetSize.y - view.getSize().y), true);
	}
}

void Camera::zoom(float direction, float dt, float windowHeightToWidth, int minZoomSizes[2], int maxZoomSizes[2], float zoomSpeed) {
	pressingZoomKey = true;
	
	if (maxZoomSizes[0] >= targetSize.x && targetSize.x >= minZoomSizes[0] && maxZoomSizes[1] >= targetSize.y && targetSize.y >= minZoomSizes[1]) {
		int dirPosOrNeg = (direction < 0.f) * 2 - 1;

		targetSize.x += dirPosOrNeg * (zoomSpeed * windowHeightToWidth) * dt;
		targetSize.y += dirPosOrNeg * zoomSpeed * dt;
	}
	else {
		if (targetSize.x < minZoomSizes[0]) {
			targetSize.x = minZoomSizes[0];
		}

		if (targetSize.y < minZoomSizes[1]) {
			targetSize.y = minZoomSizes[1];
		}

		if (targetSize.x > maxZoomSizes[0]) {
			targetSize.x = maxZoomSizes[0];
		}

		if (targetSize.y > maxZoomSizes[1]) {
			targetSize.y = maxZoomSizes[1];
		}
	}

}

void Camera::windowResize(sf::Vector2f newDimensions, float defaultCameraSizeMultiplier) {
	snapToFocus();
	
	setDimensions(newDimensions, defaultCameraSizeMultiplier);
}

std::array<sf::Vector2f, 2> Camera::getCameraBounds() {
	return { minPos, maxPos };
}

void Camera::setDimensions(sf::Vector2f newDimensions, float defaultCameraSizeMultiplier) {
	view.setSize(newDimensions.x * defaultCameraSizeMultiplier, newDimensions.y * defaultCameraSizeMultiplier);
	targetSize = view.getSize();
}

void Camera::changeAcceleration(sf::Vector2f amount, bool settingZoomVar) {
	if (settingZoomVar) {
		zoomAcc.x += amount.x;
		zoomAcc.y += amount.y;
	}
	else {
		acceleration.x += amount.x;
		acceleration.y += amount.y;
	}
}

void Camera::multAcceleration(sf::Vector2f amount, bool settingZoomVar) {
	if (settingZoomVar) {
		zoomAcc.x *= amount.x;
		zoomAcc.y *= amount.y;
	}
	else {
		acceleration.x *= amount.x;
		acceleration.y *= amount.y;
	}
}

void Camera::setAcceleration(sf::Vector2f amount, bool settingZoomVar) {
	if (settingZoomVar) {
		zoomVel.x = amount.x;
		zoomVel.y = amount.y;
	}
	else {
		acceleration.x = amount.x;
		acceleration.y = amount.y;
	}
}

sf::Vector2f Camera::getAcceleration(bool zoomVar) {
	if (zoomVar) {
		return zoomAcc;
	}
	
	return acceleration;
}

sf::Vector2f Camera::getTopLeft() {
	sf::Vector2f tl;

	tl.x = view.getCenter().x - view.getSize().x / 2.f;
	tl.y = view.getCenter().y - view.getSize().y / 2.f;

	return tl;
}

void Camera::update(sf::RenderWindow& window, float dt, settings* settings) {
	centerOnFocus(settings->cameraMoveDivisor);
	updateZoom(dt, settings->windowHeightToWidth, settings->cameraZoomDivisor);

	velocity.x += acceleration.x * dt;
	velocity.y += acceleration.y * dt;

	view.move(velocity);

	velocity.x = 0.f;
	velocity.y = 0.f;

	zoomVel.x += zoomAcc.x * dt;
	zoomVel.y += zoomAcc.y * dt;

	view.setSize(view.getSize().x + zoomVel.x, view.getSize().y + zoomVel.y);

	zoomVel.x = 0.f;
	zoomVel.y = 0.f;

	if (moveRestricted) {
		if (view.getCenter().x - view.getSize().x / 2.f < minPos.x) {
			view.setCenter(view.getSize().x / 2.f, view.getCenter().y);
		}

		if (view.getCenter().y - view.getSize().y / 2.f < minPos.y) {
			view.setCenter(view.getCenter().x, view.getSize().y / 2.f);
		}

		if (view.getCenter().x + view.getSize().x / 2.f > maxPos.x) {
			view.setCenter(maxPos.x - view.getSize().x / 2.f, view.getCenter().y);
		}

		if (view.getCenter().y + view.getSize().y / 2.f > maxPos.y) {
			view.setCenter(view.getCenter().x, maxPos.y - view.getSize().y / 2.f);
		}
	}

	pressingZoomKey = false;
	pressingMoveKey = false;

	window.setView(view);
}
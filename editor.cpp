#include "editor.h"

Editor::Editor() {}

Editor::Editor(sf::Vector2f position, AssetManager& textureManager) {
	create(position, textureManager);
}

void Editor::create(sf::Vector2f position, AssetManager& textureManager) {
	mySprite.setTexture(textureManager.texture);
	mySprite.setOrigin(mySprite.getLocalBounds().width / 2.f, mySprite.getLocalBounds().height / 2.f);
	mySprite.setPosition(position);
}

void Editor::move(sf::Vector2f offset, float dt) {
	mySprite.move(sf::Vector2f(offset.x * dt, offset.y * dt));
}

void Editor::update(sf::RenderWindow& window, Camera& camera) {}

void Editor::draw(sf::RenderWindow& window) {
	window.draw(mySprite);
}
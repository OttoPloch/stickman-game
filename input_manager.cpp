#include "input_manager.h"
#include <iostream>

InputManager::InputManager() {
	create();
}

void InputManager::create() {
	inputs.w = false;
	inputs.a = false;
	inputs.s = false;
	inputs.d = false;

	inputs.q = false;
	inputs.e = false;

	inputs.z = false;
	inputs.c = false;

	inputs.x = false;
}

void InputManager::update() {
	sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? inputs.w = true : inputs.w = false;
	sf::Keyboard::isKeyPressed(sf::Keyboard::A) ? inputs.a = true : inputs.a = false;
	sf::Keyboard::isKeyPressed(sf::Keyboard::S) ? inputs.s = true : inputs.s = false;
	sf::Keyboard::isKeyPressed(sf::Keyboard::D) ? inputs.d = true : inputs.d = false;

	sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ? inputs.q = true : inputs.q = false;
	sf::Keyboard::isKeyPressed(sf::Keyboard::E) ? inputs.e = true : inputs.e = false;

	sf::Keyboard::isKeyPressed(sf::Keyboard::Z) ? inputs.z = true : inputs.z = false;
	sf::Keyboard::isKeyPressed(sf::Keyboard::C) ? inputs.c = true : inputs.c = false;

	sf::Keyboard::isKeyPressed(sf::Keyboard::X) ? inputs.x = true : inputs.x = false;
}
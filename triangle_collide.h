#pragma once
#include <SFML/Graphics.hpp>
#include <array>

bool triangleCollide(std::array<sf::Vector2f, 3> triPoints, sf::Vector2f collidePoint);

bool triangleCollide(std::array<sf::Vector2f, 3> triPoints, sf::RectangleShape collideRect);

bool triangleCollide(std::array<sf::Vector2f, 3> triPoints, sf::IntRect collideRect);
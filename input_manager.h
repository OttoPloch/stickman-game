#pragma once
#include <SFML/Graphics.hpp>

class InputManager
{
public:
	struct inputs {
		bool w;
		bool a;
		bool s;
		bool d;

		bool q;
		bool e;

		bool z;
		bool c;

		bool x;
	};

	inputs inputs;

	InputManager();

	void create();

	void update();
};


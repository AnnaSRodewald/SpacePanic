#pragma once

#include <GL/glew.h>

struct Position{
		float x;
		float y;
		};

struct Color{
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a; //alpha
		};

struct Vertex {
	Position position; //creating an instance called position
	//float position[2];

	Color color;

	//unsigned char color[3]; //alternative
	//GLubyte color[4];  // RGB & Alpha = transparency
	//3 color & GLubyte padding; //extra byte //or 4 color
	};
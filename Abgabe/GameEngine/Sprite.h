#pragma once
#include <GL/glew.h>
#include "GLTexture.h"

#include <string>

namespace GameEngine{
	class Sprite
		{
		public:
			Sprite(void);
			~Sprite(void);

			void init(float x, float y, float width, float height, std::string texturePath);

			void draw();

		private:
			float m_x;
			float m_y;
			float m_width;
			float m_height;
			GLuint m_vboID;
			GLTexture m_texture;
		};

	}
#pragma once
#include "TextureCache.h"
#include <string>

namespace GameEngine{

	class ResourceManager
		{
		public:
			static GLTexture getTexture(std::string texturePath);

			//static GLuint boundTexture;

		private:
			static TextureCache m_textureCache;
		};

	}
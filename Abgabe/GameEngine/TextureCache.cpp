#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>


namespace GameEngine{

	TextureCache::TextureCache(void)
		{
		}


	TextureCache::~TextureCache(void)
		{
		}


	GLTexture TextureCache::getTexture(std::string texturePath){
		//lookup the texture and see if its in the map
		//std::map<std::string, GLTexture>::iterator mit = m_textureMap.find(texturePath); 
		//same as
		auto mit = m_textureMap.find(texturePath);

		//check if its not in the  map
		if (mit == m_textureMap.end()){
			//load the texture
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			//Insert it into the map
			//std::pair<std::string, GLTexture> newPair(texturePath, newTexture);
			//m_textureMap.insert(newPair);
			//same as
			m_textureMap.insert(make_pair(texturePath, newTexture));

			//std::cout << "Loaded Texture!\n";
			return newTexture;
			}

		//std::cout << "Used Cached Texture!\n";
		return mit->second;
		}

	}
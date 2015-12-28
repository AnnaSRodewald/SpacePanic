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
		//std::map<std::string, GLTexture>::iterator mit = _textureMap.find(texturePath); 
		//same as
		auto mit = _textureMap.find(texturePath);

		//check if its not in the  map
		if (mit == _textureMap.end()){
			//load the texture
			GLTexture newTexture = ImageLoader::loadPNG(texturePath);

			//Insert it into the map
			//std::pair<std::string, GLTexture> newPair(texturePath, newTexture);
			//_textureMap.insert(newPair);
			//same as
			_textureMap.insert(make_pair(texturePath, newTexture));

			//std::cout << "Loaded Texture!\n";
			return newTexture;
			}

		//std::cout << "Used Cached Texture!\n";
		return mit->second;
		}

	}
#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "Vertex.h"

namespace GameEngine{

	enum class GlypthSortType{
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
		};

	class Glypth {
	public:
		Glypth();
		Glypth(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) : texture(Texture), depth(Depth){

			topLeft.color = color;
			topLeft.setPosition(destRect.x, destRect.y + destRect.w);
			topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

			bottomLeft.color = color;
			bottomLeft.setPosition(destRect.x, destRect.y);
			bottomLeft.setUV(uvRect.x, uvRect.y);

			topRight.color = color;
			topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
			topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

			bottomRight.color = color;
			bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
			bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
		}

		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;

		};

	class RenderBatch {
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : 
			offset(Offset), 
			numVertices(NumVertices), 
			texture(Texture){
			}

		GLuint offset;
		GLuint numVertices;
		GLuint texture;
		};

	class SpriteBatch
		{
		public:
			SpriteBatch(void);
			~SpriteBatch(void);

			void init(); //1

			void begin(GlypthSortType sortType = GlypthSortType::TEXTURE); //2
			void end(); //4

			void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color); //3

			void renderBatch(); //5

		private: 
			void createRenderBatches();
			void createVertexArray();
			void sortGlypths();

			static bool compareFrontToBack(Glypth* a, Glypth* b);
			static bool compareBackToFront(Glypth* a, Glypth* b);
			static bool compareTexture(Glypth* a, Glypth* b);

			GLuint _vbo;
			GLuint _vao;

			GlypthSortType _sortType;

			std::vector<Glypth*> _glypthPointers; ///< This is for sorting
			std::vector<Glypth> _glypths; ///< these are the actual glypths
			std::vector<RenderBatch> _renderBatches;
		};


	}
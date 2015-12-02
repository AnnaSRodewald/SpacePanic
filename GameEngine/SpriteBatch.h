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

	struct Glypth {

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

			void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture,  float depth, const Color& color); //3

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

			std::vector<Glypth*> _glypths;
			std::vector<RenderBatch> _renderBatches;
		};


	}
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

		Glypth(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color);

		Glypth(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle);


		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;

	private:
		glm::vec2 rotatePoint(glm::vec2 position, float angle);

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

			void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle); //3

			void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, glm::vec2& dir); //3

			void renderBatch(); //5

		private: 
			void createRenderBatches();
			void createVertexArray();
			void sortGlypths();

			static bool compareFrontToBack(Glypth* a, Glypth* b);
			static bool compareBackToFront(Glypth* a, Glypth* b);
			static bool compareTexture(Glypth* a, Glypth* b);

			GLuint m_vbo;
			GLuint m_vao;

			GlypthSortType m_sortType;

			std::vector<Glypth*> m_glypthPointers; ///< This is for sorting
			std::vector<Glypth> m_glypths; ///< these are the actual glypths
			std::vector<RenderBatch> m_renderBatches;
		};


	}
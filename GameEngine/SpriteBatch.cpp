#include "SpriteBatch.h"
#include <algorithm>

namespace GameEngine{

	SpriteBatch::SpriteBatch(void) : _vbo(0), _vao(0)
		{
		}


	SpriteBatch::~SpriteBatch(void)
		{
		}


	void SpriteBatch::init(){
		createVertexArray();

		}

	void SpriteBatch::begin(GlypthSortType sortType) /* = GlypthSortType::TEXTURE) */{
		_sortType = sortType;
		_renderBatches.clear();	
		_glypths.clear();
		}

	void SpriteBatch::end(){
		//Set up all pointers for fast sorting
		_glypthPointers.resize(_glypths.size());
		for (int i = 0; i < _glypths.size(); i++)
		{
			_glypthPointers[i] = &_glypths[i];
		}
		sortGlypths();
		createRenderBatches();

		}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color){

		_glypths.emplace_back(destRect, uvRect, texture, depth, color);
		}

	void SpriteBatch::renderBatch(){

		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); i++)
			{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
			}

		glBindVertexArray(0);
		}

	void SpriteBatch::createRenderBatches(){
		std::vector<Vertex> vertices;
		vertices.resize(_glypthPointers.size() * 6);

		if (_glypthPointers.empty())
			{
			return;
			}

		int offset = 0;
		int cv = 0; //current vertex

		_renderBatches.emplace_back(offset, 6, _glypthPointers[0]->texture);
		vertices[cv++]= _glypthPointers[0]->topLeft;
		vertices[cv++]= _glypthPointers[0]->bottomLeft;
		vertices[cv++]= _glypthPointers[0]->bottomRight;
		vertices[cv++]= _glypthPointers[0]->bottomRight;
		vertices[cv++]= _glypthPointers[0]->topRight;
		vertices[cv++]= _glypthPointers[0]->topLeft;
		offset += 6;

		for (int cg = 1; cg < _glypthPointers.size(); cg++) //cg = current Glypth
			{
			if (_glypthPointers[cg]->texture != _glypthPointers[cg - 1]->texture)
				{
				_renderBatches.emplace_back(offset, 6, _glypthPointers[cg]->texture);
				} else{
					_renderBatches.back().numVertices += 6;
				}
			vertices[cv++]= _glypthPointers[cg]->topLeft;
			vertices[cv++]= _glypthPointers[cg]->bottomLeft;
			vertices[cv++]= _glypthPointers[cg]->bottomRight;
			vertices[cv++]= _glypthPointers[cg]->bottomRight;
			vertices[cv++]= _glypthPointers[cg]->topRight;
			vertices[cv++]= _glypthPointers[cg]->topLeft;
			offset += 6;
			}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW); faster way -->
		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);




		}

	void SpriteBatch::createVertexArray(){

		if (_vao == 0)
			{
			glGenVertexArrays(1, &_vao);
			}

		glBindVertexArray(_vao);

		if (_vbo == 0)
			{
			glGenBuffers(1, &_vbo);
			}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);


		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//tells openGl where the data is
		//this is the position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		//This is the color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		//This is the UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);

		}

	void SpriteBatch::sortGlypths(){
		switch (_sortType)
			{
			case GameEngine::GlypthSortType::FRONT_TO_BACK:
				std::stable_sort(_glypthPointers.begin(), _glypthPointers.end(), compareFrontToBack);
				break;
			case GameEngine::GlypthSortType::BACK_TO_FRONT:
				std::stable_sort(_glypthPointers.begin(), _glypthPointers.end(), compareFrontToBack);
				break;
			case GameEngine::GlypthSortType::TEXTURE:
				std::stable_sort(_glypthPointers.begin(), _glypthPointers.end(), compareTexture);
				break;
			default:
				break;
			}
		}

	bool SpriteBatch::compareFrontToBack(Glypth* a, Glypth* b){
		return (a->depth < b->depth);
		}

	bool SpriteBatch::compareBackToFront(Glypth* a, Glypth* b){
		return (a->depth > b->depth);
		}

	bool SpriteBatch::compareTexture(Glypth* a, Glypth* b){
		return (a->texture < b->texture);
		}


	}
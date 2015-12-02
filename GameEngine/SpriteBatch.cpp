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
		for (int i = 0; i < _glypths.size(); i++)
			{
			delete _glypths[i];
			}
		_glypths.clear();

		}
	void SpriteBatch::end(){
		sortGlypths();
		createRenderBatches();

		}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color){

		Glypth* newGlypth = new Glypth;
		newGlypth->texture = texture;
		newGlypth->depth = depth;

		newGlypth->topLeft.color = color;
		newGlypth->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newGlypth->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlypth->bottomLeft.color = color;
		newGlypth->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlypth->bottomLeft.setUV(uvRect.x, uvRect.y);

		newGlypth->topRight.color = color;
		newGlypth->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlypth->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		newGlypth->bottomRight.color = color;
		newGlypth->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlypth->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		_glypths.push_back(newGlypth);
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
		vertices.resize(_glypths.size() * 6);

		if (_glypths.empty())
			{
			return;
			}

		int offset = 0;
		int cv = 0; //current vertex

		_renderBatches.emplace_back(offset, 6, _glypths[0]->texture);
		vertices[cv++]= _glypths[0]->topLeft;
		vertices[cv++]= _glypths[0]->bottomLeft;
		vertices[cv++]= _glypths[0]->bottomRight;
		vertices[cv++]= _glypths[0]->bottomRight;
		vertices[cv++]= _glypths[0]->topRight;
		vertices[cv++]= _glypths[0]->topLeft;
		offset += 6;

		for (int cg = 1; cg < _glypths.size(); cg++) //cg = current Glypth
			{
			if (_glypths[cg]->texture != _glypths[cg - 1]->texture)
				{
				_renderBatches.emplace_back(offset, 6, _glypths[cg]->texture);
				} else{
					_renderBatches.back().numVertices += 6;
				}
			vertices[cv++]= _glypths[cg]->topLeft;
			vertices[cv++]= _glypths[cg]->bottomLeft;
			vertices[cv++]= _glypths[cg]->bottomRight;
			vertices[cv++]= _glypths[cg]->bottomRight;
			vertices[cv++]= _glypths[cg]->topRight;
			vertices[cv++]= _glypths[cg]->topLeft;
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
				std::stable_sort(_glypths.begin(), _glypths.end(), compareFrontToBack);
				break;
			case GameEngine::GlypthSortType::BACK_TO_FRONT:
				std::stable_sort(_glypths.begin(), _glypths.end(), compareFrontToBack);
				break;
			case GameEngine::GlypthSortType::TEXTURE:
				std::stable_sort(_glypths.begin(), _glypths.end(), compareTexture);
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
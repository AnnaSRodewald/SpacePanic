#pragma once

#include <string>
#include <GL/glew.h>

class GLSLProgram
	{
	public:
		GLSLProgram(void);
		~GLSLProgram(void);

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragementShaderFilePath);

		void linkShaders();

		void addAttribute(const std::string& attributeName);

		void use();

		void unuse();

	private:
		int _numAttributes;

		GLuint _programID;

		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;

		void compileShaders(const std::string& filePath, GLuint shaderID);
	};


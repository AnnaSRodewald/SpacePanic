#pragma once

#include <string>
#include <GL/glew.h>

namespace GameEngine{

	class GLSLProgram
		{
		public:
			GLSLProgram(void);
			~GLSLProgram(void);

			void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);

			void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragementShaderFilePath);

			void linkShaders();

			void addAttribute(const std::string& attributeName);

			GLint getUniformLocation(const std::string& uniformName);

			void use();

			void unuse();

			void dispose();


		private:
			int m_numAttributes;

			void compileShaders(const char* source, const std::string& name, GLuint shaderID);

			/*void compileShaders(const std::string& filePath, GLuint shaderID);*/

			GLuint m_programID;

			GLuint m_vertexShaderID;
			GLuint m_fragmentShaderID;




		};

	}
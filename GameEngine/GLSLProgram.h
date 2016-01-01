#pragma once

#include <string>
#include <GL/glew.h>

namespace GameEngine{

	class GLSLProgram
		{
		public:
			GLSLProgram(void);
			~GLSLProgram(void);

			void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragementShaderFilePath);

			void linkShaders();

			void addAttribute(const std::string& attributeName);

			GLint getUniformLocation(const std::string& uniformName);

			void use();

			void unuse();


		private:
			int m_numAttributes;

			void compileShaders(const std::string& filePath, GLuint shaderID);

			GLuint m_programID;

			GLuint m_vertexShaderID;
			GLuint m_fragmentShaderID;




		};

	}
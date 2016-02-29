#include "GLSLProgram.h"
#include "GameEngineErrors.h"
#include "IOManager.h"

#include <fstream>

#include <vector>

namespace GameEngine{

	GLSLProgram::GLSLProgram(void) :  m_numAttributes(0), m_programID(0), m_vertexShaderID(0), m_fragmentShaderID(0)
		{
		}


	GLSLProgram::~GLSLProgram(void)
		{
		}

	void GLSLProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource){

		//Get a program object.
		m_programID = glCreateProgram();

		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == 0) {
			fatalError("Vertex shader failed to be created!");
		}

		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == 0) {
			fatalError("Fragment shader failed to be created!");
		}

		compileShaders(vertexSource, "Vertex Shader", m_vertexShaderID);
		compileShaders(fragmentSource, "Fragment Shader", m_fragmentShaderID);

	}


	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragementShaderFilePath){

		std::string vertSource;
		std::string fragSource;

		IOManager::readFileToBuffer(vertexShaderFilePath, vertSource);
		IOManager::readFileToBuffer(fragementShaderFilePath, fragSource);

		compileShadersFromSource(vertSource.c_str(), fragSource.c_str());
		}

	void GLSLProgram::linkShaders(){
		//Vertex and fragment shaders are successfully compiled.
		//Now time to link them together into a program.

		//Attach our shaders to our program
		glAttachShader(m_programID, m_vertexShaderID);
		glAttachShader(m_programID, m_fragmentShaderID);

		//Link our program
		glLinkProgram(m_programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, (int *)&isLinked);
		if(isLinked == GL_FALSE)
			{
			GLint maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(m_programID);
			//Don't leak shaders either.
			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shaders failed to link!");
			}

		//Always detach shaders after a successful link.
		glDetachShader(m_programID, m_vertexShaderID);
		glDetachShader(m_programID, m_fragmentShaderID);

		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
		}

	void GLSLProgram::addAttribute(const std::string& attributeName){
		//layout(location = 2) in vec4 a_vec; for newer models of opengl -- see https://www.opengl.org/wiki/Vertex_Shader#Inputs

		glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
		}

	GLint GLSLProgram::getUniformLocation(const std::string& uniformName){
		GLint location = glGetUniformLocation(m_programID, uniformName.c_str());

		if (location == GL_INVALID_INDEX){
			fatalError("Uniform " + uniformName + " not found in shader!");
			}

		return location;
		}


	void GLSLProgram::use(){
		glUseProgram(m_programID);
		for (int i = 0; i < m_numAttributes; i++)
			{
			glEnableVertexAttribArray(i);
			}
		}

	void GLSLProgram::unuse(){
		glUseProgram(0);
		for (int i = 0; i < m_numAttributes; i++)
			{
			glDisableVertexAttribArray(i);
			}
		}

	void GLSLProgram::dispose(){
		if (m_programID)
		{
			glDeleteProgram(m_programID);
		}
	}


	void GLSLProgram::compileShaders(const char* source, const std::string& name, GLuint shaderID){

		glShaderSource(shaderID, 1, &source, nullptr);

		glCompileShader(shaderID);

		//test if the shader has (compile)errors
		GLint isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);

		if(isCompiled == GL_FALSE)
			{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(shaderID); // Don't leak the shader.

			std::printf("%s\n", &(errorLog[0]));
			fatalError("Shader " + name +" failed to compile!");
			}

		}
	}
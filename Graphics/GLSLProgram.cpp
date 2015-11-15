#include "GLSLProgram.h"
#include "Errors.h"

#include <fstream>

#include <vector>

GLSLProgram::GLSLProgram(void) :  _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{
	}


GLSLProgram::~GLSLProgram(void)
	{
	}


void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragementShaderFilePath){
	//Get a program object.
	_programID = glCreateProgram();

	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0) {
		fatalError("Vertex shader failed to be created!");
		}

	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0) {
		fatalError("Fragment shader failed to be created!");
		}

	compileShaders(vertexShaderFilePath, _vertexShaderID);
	compileShaders(fragementShaderFilePath, _fragmentShaderID);
	}

void GLSLProgram::linkShaders(){
	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.

	//Attach our shaders to our program
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	//Link our program
	glLinkProgram(_programID);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
		{
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

		//We don't need the program anymore.
		glDeleteProgram(_programID);
		//Don't leak shaders either.
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shaders failed to link!");
		}

	//Always detach shaders after a successful link.
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
	}

void GLSLProgram::addAttribute(const std::string& attributeName){
	//layout(location = 2) in vec4 a_vec; for newer models of opengl -- see https://www.opengl.org/wiki/Vertex_Shader#Inputs

	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

void GLSLProgram::use(){
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++)
		{
		glEnableVertexAttribArray(i);
		}
	}

void GLSLProgram::unuse(){
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++)
		{
		glDisableVertexAttribArray(i);
		}
	}


void GLSLProgram::compileShaders(const std::string& filePath, GLuint shaderID){

	std::fstream shaderFile(filePath);
	if(shaderFile.fail()){
		perror(filePath.c_str());
		fatalError("Failed to open " + filePath);
		}

	std::string fileContents = "";
	std::string line;

	while (std::getline(shaderFile, line))
		{
		fileContents += line + "\n";
		}

	shaderFile.close();

	const char* contentsPtr = fileContents.c_str();
	glShaderSource(shaderID, 1, &contentsPtr, nullptr);

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
		fatalError("Shader " + filePath +" failed to compile!");
		}

	}
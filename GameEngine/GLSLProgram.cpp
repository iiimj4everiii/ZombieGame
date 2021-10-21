#include <fstream>
#include <iostream>
#include <vector>

#include "GLSLProgram.h"
#include "Errors.h"

namespace GameEngine
{
	//Using initialization list instead of the old way of initializing variables to 0
	GLSLProgram::GLSLProgram() : _numAttributes(0), _vertexShaderID(0), _fragmentShaderID(0), _programID(0)
	{
		//ctor
	}

	GLSLProgram::~GLSLProgram()
	{
		//dtor
	}

	void GLSLProgram::compileShaders(const std::string &vertShaderFilePath, const std::string &fragShaderFilePath)
	{
		//Get a program object
		_programID = glCreateProgram();

		//Creating a GL Shader for vertex
		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0)
		{
			fatalError("Vertex shader failed to be created!");
		}
		compileEachShader(vertShaderFilePath, _vertexShaderID);

		//Creating a GL Shader for fragment
		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0)
		{
			fatalError("Fragment shader failed to be created!");
		}
		compileEachShader(fragShaderFilePath, _fragmentShaderID);

		return;
	}

	void GLSLProgram::compileEachShader(const std::string &shaderFilePath, GLuint shaderID)
	{
		std::string shaderContents = "";
		std::string stringGetline;

		std::ifstream infileShader(shaderFilePath);
		if (infileShader.fail())
		{
			//perror prints out more error details
			perror(shaderFilePath.c_str());
			fatalError("Failed to open " + shaderFilePath);
		}

		while (std::getline(infileShader, stringGetline))
		{
			//Appending or concatenating vertexFileContents with new getline read in
			//Need to add "\n" as getline() doesn't add new line at the end of each read line
			shaderContents += stringGetline + "\n";
		}

		infileShader.close();

		//char* is an array of characters (in C)
		const char* shaderContentsPtr = shaderContents.c_str();
		glShaderSource(shaderID, 1, &shaderContentsPtr, nullptr);

		//Compile shader
		glCompileShader(shaderID);

		//Compilation error checking
		GLint success = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxlength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxlength);

			//The maxLength includes the NULL character
			std::vector<char> errorLog(maxlength);
			//&errorLog[0] interprets the array (vector) of char in errorLog as a string
			glGetShaderInfoLog(shaderID, maxlength, &maxlength, &errorLog[0]);

			//We don't need the shader anymore.
			glDeleteShader(shaderID);

			//&errorLog[0] interprets the array (vector) of char in errorLog as a string
			std::printf("%s\n", &errorLog[0]);
			fatalError("Shader " + shaderFilePath + " failed to compile!");
		}

		return;
	}

	void GLSLProgram::addAttribute(const std::string& attributeName)
	{
		glBindAttribLocation(_programID, _numAttributes, attributeName.c_str());
		_numAttributes++;

		return;
	}

	GLint GLSLProgram::getUniformLocation(const std::string uniformName)
	{
		GLint uniformLocation = glGetUniformLocation(_programID, uniformName.c_str());

		if ((GLuint)uniformLocation == GL_INVALID_INDEX)
		{
			fatalError("Uniform " + uniformName + " not found in shader files!");
		}

		return uniformLocation;
	}

	void GLSLProgram::linkShaders()
	{
		//Attach our shaders to our program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		//Link our program
		glLinkProgram(_programID);

		//Note the different functions here: glGetProgram* instead of glGetShader*
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_LINK_STATUS, &maxLength);

			//The maxLength includes the NULL character
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(_programID);
			//Don't leak shaders either
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			//Use the infolog as you see fit
			std::printf("%s\n", &errorLog[0]);
			fatalError("Shader failed to link!");
		}

		//Always detach and delete shaders after a successful link.
		glDetachShader(_programID, _vertexShaderID);
		glDeleteShader(_vertexShaderID);

		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_fragmentShaderID);

		return;
	}

	void GLSLProgram::use()
	{
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}

		return;
	}

	void GLSLProgram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}

		return;
	}
}
#pragma once
#include <string>
#include <GL/glew.h>

namespace GameEngine
{
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string &vertShaderFilePath, const std::string &fragShaderFilePath);
		void linkShaders();
		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string uniformName);

		void use();
		void unuse();

	private:
		void compileEachShader(const std::string &shaderFilePath, GLuint shaderID);
		int _numAttributes;

		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
		GLuint _programID;
	};
}
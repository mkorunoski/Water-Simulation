#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
private:
	GLuint vertex;
	GLuint fragment;
	GLuint program;

	std::string shaderName;
	
	GLuint CompileShader(const std::string& path, GLuint shaderType, const std::string& name)
	{
		std::string code;

		std::ifstream shaderFile;

		shaderFile.exceptions(std::ifstream::badbit);
		try
		{
			shaderFile.open(path);

			std::stringstream shaderStream;

			shaderStream << shaderFile.rdbuf();

			shaderFile.close();

			code = shaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* shaderCode = code.c_str();

		GLuint shader;
		GLint success;
		GLchar infoLog[512];

		shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);			
			std::cout << "ERROR::SHADER:" + shaderName + ":" + name + "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		return shader;
	}

	void LinkProgram(GLuint shaders[], GLuint numShaders)
	{
		GLint success;
		GLchar infoLog[512];

		program = glCreateProgram();

		for (GLuint i = 0; i < numShaders; ++i)
			glAttachShader(program, shaders[i]);

		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER:" + shaderName + ":" + "PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		for (GLuint i = 0; i < numShaders; ++i)
			glDeleteShader(shaders[i]);
	}

public:
	Shader() { }

	Shader& operator=(const Shader& shader)
	{
		vertex	 = shader.vertex;		
		fragment = shader.fragment;
		program  = shader.program;
		shaderName.assign(shader.shaderName);
		return *this;
	}

	const GLuint& GetProgram() { return program; }

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const std::string& shaderName)
	{
		this->shaderName.assign(shaderName);
		vertex	 = CompileShader(vertexPath, GL_VERTEX_SHADER, "VERTEX");
		fragment = CompileShader(fragmentPath, GL_FRAGMENT_SHADER, "FRAGMENT");

		LinkProgram(new GLuint[] {vertex, fragment}, 2);
	}
		
	void Use()
	{
		glUseProgram(program);
	}

	void Unuse()
	{
		glUseProgram(0);
	}
};

#endif
/*
* shaders.cpp
* This file contains implementations for graphics shaders
* author      :  Jake Sheehan
* institution :  Southern New Hampshire University
* professor   :  Kurt Diesch
* date        :  September 9, 2021
*
* References  :
* This code is largely the result of following along
* with the reading at learnopengl.com, which is licensed
* under the terms of Creative Commons CC BY-NC 4.0.
*/

#include "shaders.h"

namespace shaders
{
	Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		// --------------- VARIABLES ---------------
		std::ifstream vertexFile;
		std::string vertexSourceString;
		const GLchar* vertexSource;
		std::ifstream fragmentFile;
		std::string fragmentSourceString;
		const GLchar* fragmentSource;
		std::string line;
		int success;
		char infolog[512];

		// --------------- FILES ---------------
		// Opens files
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);

		// Read vertex source
		if (vertexFile)
		{
			while (std::getline(vertexFile, line))
			{
				vertexSourceString += line;
				vertexSourceString.push_back('\n');
			}
			vertexSourceString.pop_back(); // removes final newline
		}
		else
		{
			std::cout << "Error: vertex shader file failed to open" << std::endl;
		}

		line = ""; // Reset line

		// Read fragment source
		if (fragmentFile)
		{
			while (std::getline(fragmentFile, line))
			{
				fragmentSourceString += line;
				fragmentSourceString.push_back('\n');
			}
			fragmentSourceString.pop_back(); // removes final newline
		}
		else
		{
			std::cout << "Error: fragment shader file failed to open" << std::endl;
		}
		
		// Converts source codes from string to c_string
		vertexSource = vertexSourceString.c_str();
		fragmentSource = fragmentSourceString.c_str();

		// --------------- SHADER PROGRAM ---------------
		// Creates shader program object
		GLuint shaderProgram = glCreateProgram();

		// Creates vertex and fragment shader objects
		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Gets shader source
		glShaderSource(vertexShaderID, 1, &vertexSource, NULL);
		glShaderSource(fragmentShaderID, 1, &fragmentSource, NULL);

		// Builds vertex shader and prints any errors
		glCompileShader(vertexShaderID);
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShaderID, 512, NULL, infolog);
			std::cout << "ERROR: vertex shader failed to compile\n" << infolog << std::endl;
		}

		// Builds fragment shader and prints any errors
		glCompileShader(fragmentShaderID);
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShaderID, 512, NULL, infolog);
			std::cout << "ERROR: fragment shader failed to compile\n" << infolog << std::endl;
		}

		// Attaches compiled shaders to program
		glAttachShader(shaderProgram, vertexShaderID);
		glAttachShader(shaderProgram, fragmentShaderID);

		// Links program and prints any errors
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
			std::cout << "ERROR: shader program failed to link\n" << infolog << std::endl;
		}
		// Delete shaders after creating shader program because they are no longer needed
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

		ID = shaderProgram;
	}

	void Shader::use()
	{
		glUseProgram(ID);
	}
}
/*
* shaders.h
* This file contains declarations for graphics shaders
* author      :  Jake Sheehan
* institution :  Southern New Hampshire University
* professor   :  Kurt Diesch
* date        :  September 1, 2021
*
* References  :
* This code is largely the result of following along
* with the reading at learnopengl.com, which is licensed
* under the terms of Creative Commons CC BY-NC 4.0.
*/

#pragma once
#include <GLAD/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace shaders
{
	class Shader
	{
	public:
		GLuint ID;
		Shader() : ID{ 1000 } {}
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
		void use();
	};
}

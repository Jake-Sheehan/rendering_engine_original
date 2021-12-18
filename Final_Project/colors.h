/*
* colors.h
* This file declares RGB colors
* author      :  Jake Sheehan
* institution :  Southern New Hampshire University
* professor   :  Kurt Diesch
* date        :  October 13, 2021
*
* References  :
* This code is largely the result of following along
* with the reading at learnopengl.com, which is licensed
* under the terms of Creative Commons CC BY-NC 4.0.
*/

#pragma once
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <tuple>

namespace colors
{
	struct color
	{
		GLfloat r, g, b, a;
		// constructor
		color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
		{
			// These ternary operations just ensure that the values are between 1.0 and 0.0
			// If greater than 1.0 then it assigns 1.0, if less than 0.0 it assigns 0.0
			r = (red >= 0.0f && red <= 1.0f) ? red : (red > 1.0f) ? 1.0f : 0.0f;
			g = (green >= 0.0f && green <= 1.0f) ? green : (green > 1.0f) ? 1.0f : 0.0f;
			b = (blue >= 0.0f && blue <= 1.0f) ? blue : (blue > 1.0f) ? 1.0f : 0.0f;
			a = (alpha >= 0.0f && alpha <= 1.0f) ? alpha : (alpha > 1.0f) ? 1.0f : 0.0f;
		}
		// Overloads * operator to multiply colors with eachother
		color operator*(const color otherColor)
		{
			return color(r * otherColor.r, g * otherColor.g, b * otherColor.b, a * otherColor.a);
		}
		// Overloads * operator to multiply color by float to change strength 
		color operator*(GLfloat strength)
		{
			return color(r * strength, g * strength, b * strength, a * strength);
		}
	};

	// Colors
	extern color sky_blue;
	extern color sunlight;
	extern color white;
}


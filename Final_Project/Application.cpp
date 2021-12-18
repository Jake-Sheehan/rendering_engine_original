/*
* Application.cpp
* This file creates a window and draws two triangles
* author      :  Jake Sheehan
* institution :  Southern New Hampshire University
* professor   :  Kurt Diesch
* date        :  October 24, 2021
* 
* References  :
* This code is largely the result of following along
* with the reading at learnopengl.com, which is licensed
* under the terms of Creative Commons CC BY-NC 4.0.
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/normal.hpp>
#include "shaders.h"
#include "mesh.h"
#include "setup.h"
#include "input.h"
#include "colors.h"
#include "model.h"
#include <vector>
#include <chrono>
#include <thread>

int main()
{
	// -------------------- INITIALIZATION --------------------

	// Initializes window
	const unsigned int SCREEN_WIDTH = 960;
	const unsigned int SCREEN_HEIGHT = 540;
	GLfloat aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	GLFWwindow* window = setup::initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	// -------------------- SHADER PROGRAMS --------------------
	// Creates light source shader program so that light source will not be effected
	// by ambient light
	const GLchar* vertexShaderPath = "shader_source/light_source_vertex_shader.txt";
	const GLchar* fragmentShaderPath = "shader_source/light_source_fragment_shader.txt";
	shaders::Shader lightSourceShader = shaders::Shader(vertexShaderPath, fragmentShaderPath);

	// Creates object shader program that is effected by ambient light
	vertexShaderPath = "shader_source/vertex_shader.txt";
	fragmentShaderPath = "shader_source/fragment_shader.txt";
	shaders::Shader objectShader = shaders::Shader(vertexShaderPath, fragmentShaderPath);

	// -------------------- LIGHTING --------------------
	// This will be encapsulated in a class object moving forward
	objectShader.use();

	// Light source position
	glm::vec3 lightPos{ 20.0f, 20.0f, 20.0f };

	// Sets light uniforms
	GLuint lightPosLoc = glGetUniformLocation(objectShader.ID, "light.position");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

	GLuint lightAmbientLoc = glGetUniformLocation(objectShader.ID, "light.ambient");
	glUniform4f(lightAmbientLoc, 0.1f, 0.1f, 0.1f, 1.0f);

	GLuint lightDiffuseLoc = glGetUniformLocation(objectShader.ID, "light.diffuse");
	glUniform4f(lightDiffuseLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	GLuint lightSpecularLoc = glGetUniformLocation(objectShader.ID, "light.specular");
	glUniform4f(lightSpecularLoc, 1.0f, 1.0f, 1.0f, 1.0f);

	GLfloat ambientStrength = 1.0f;

	// Sets camera position
	GLuint viewPosLoc = glGetUniformLocation(objectShader.ID, "viewPos");
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(input::cameraPos));

	// Sets ambient strength of light source
	lightSourceShader.use();
	GLuint lightSourceAmbientStrenthLoc = glGetUniformLocation(lightSourceShader.ID, "ambientStrength");
	glUniform1f(lightSourceAmbientStrenthLoc, ambientStrength);

	mesh::TriangleMesh light{
		std::vector<GLfloat>{
			// POSITION              // Normals        // Texture
			-0.25f,  0.25f,  0.25f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 0 - Top left front
			 0.25f,  0.25f,  0.25f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 1 - Top right front
			-0.25f, -0.25f,  0.25f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 2 - Bottom left front
			 0.25f, -0.25f,  0.25f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 3 - Bottom right front

			-0.25f,  0.25f, -0.25f,  0.0f, 0.0f, 0.0f, 3.0f, 1.0f, // 4 - Top left back
			 0.25f,  0.25f, -0.25f,  0.0f, 0.0f, 0.0f, 2.0f, 1.0f, // 5 - Top right back
			-0.25f, -0.25f, -0.25f,  0.0f, 0.0f, 0.0f, 3.0f, 0.0f, // 6 - Bottom left back
			 0.25f, -0.25f, -0.25f,  0.0f, 0.0f, 0.0f, 2.0f, 0.0f, // 7 - Bottom right back

			// Top
			-0.25f,  0.25f,  0.25f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 8 - Top left front
			 0.25f,  0.25f,  0.25f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 9 - Top right front
			-0.25f,  0.25f, -0.25f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 10 - Top left back
			 0.25f,  0.25f, -0.25f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 11 - Top right back

			// Bottom
			-0.25f, -0.25f,  0.25f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 12 - Bottom left front
			 0.25f, -0.25f,  0.25f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 13 - Bottom right front
			-0.25f, -0.25f, -0.25f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 14 - Bottom left back
			 0.25f, -0.25f, -0.25f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 15 - Bottom right back
		},
		std::vector<GLushort>{
			0, 1, 2,    // triangle 1 front
			1, 2, 3,    // triangle 2 front
			4, 5, 6,    // triangle 1 back
			5, 6, 7,    // triangle 2 back
			8, 9, 10,   // triangle 1 top
			9, 10, 11,  // triangle 2 top
			12, 13, 14, // triangle 1 bottom
			13, 14, 15, // triangle 2 bottom
			0, 2, 6,    // triangle 1 left
			0, 4, 6,    // triangle 2 left
			1, 3, 5, // triangle 1 right
			3, 5, 7  // triangle 2 right
		},
		lightSourceShader,
		"textures/light_01.jpg",
		glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
		1.0f
	};

	light.translate(lightPos.x, lightPos.y, lightPos.z);
	light.scale(5.0f, 5.0f, 5.0f);


	
	// -------------------- Projection --------------------
	glm::mat4 projection = glm::perspective(
		glm::radians(45.0f),
		aspect,
		0.1f,
		100.0f);

	// Sets perspective in light source shader
	lightSourceShader.use();
	GLuint lightSourceProjectionLoc = glGetUniformLocation(lightSourceShader.ID, "projection"); // Get projection location
	GLuint lightSourceViewLoc = glGetUniformLocation(lightSourceShader.ID, "view"); // Get view location
	glUniformMatrix4fv(lightSourceProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// Sets perspective in object shader
	objectShader.use();
	GLuint projectionLoc = glGetUniformLocation(objectShader.ID, "projection"); // Get projection location
	GLuint viewLoc = glGetUniformLocation(objectShader.ID, "view"); // Get view location
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// -------------------- SCENE OBJECTS --------------------

	mesh::TriangleMesh table{
		std::vector<GLfloat>{
		// Position            // Normals        // Texture
		-25.0f, 0.0f, -25.0f,  0.0f, 1.0f, 0.0f, 0.0f, 12.0f, // 0 - back left
		 25.0f, 0.0f, -25.0f,  0.0f, 1.0f, 0.0f, 12.0f, 12.0f, // 1 - back right
		-25.0f, 0.0f,  25.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 2 - front left
		 25.0f, 0.0f,  25.0f,  0.0f, 1.0f, 0.0f, 12.0f, 0.0f  // 3 - front right
	},
	std::vector<GLushort>{
		0, 2, 3,
		0, 1, 3
	},
	objectShader,
	"textures/wood_table_01.jpg",
	glm::vec4(0.25f, 0.25f, 0.25f, 1.0f),
	1.0f
	};

	const char* bookPath = "models/book/book.obj";
	model::Model book = model::Model(bookPath);
	book.translate(0.0f, 0.0f, 10.0f);
	book.rotate(-90.0f, 'y');

	const char* headphonesPath = "models/headphones/headphones.obj";
	model::Model headphones = model::Model(headphonesPath);
	headphones.translate(5.0f, 0.5f, 6.0f);
	headphones.rotate(225.0f, 'y');

	const char* penPath = "models/pen/pen.obj";
	model::Model pen = model::Model(penPath);
	pen.translate(-2.0f, 0.25f, 11.0f);

	const char* cupPath = "models/cup/cup.obj";
	model::Model cup = model::Model(cupPath);
	cup.translate(-5.0f, 0.1f, 5.0f);
	cup.rotate(180.0f, 'y');


	// ~~~~~~~~~~~~~~~~~~~~ RENDER LOOP ~~~~~~~~~~~~~~~~~~~~~~~
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	while (!glfwWindowShouldClose(window))
	{
		// -------------------- RENDER --------------------
	
		// Enable Z-depth testing to test which objects are covered by others
		glEnable(GL_DEPTH_TEST);
		// Accept the closer fragment
		glDepthFunc(GL_LESS);
		
		// Clears frame and Z buffers
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Sends view informtion to uniform variables in shaders
		lightSourceShader.use();
		glUniformMatrix4fv(lightSourceViewLoc, 1, GL_FALSE, glm::value_ptr(input::view)); // updates view
		objectShader.use();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(input::view));
		glUniform3fv(viewPosLoc, 1, glm::value_ptr(input::cameraPos)); // updates view position
		
		// Draws shapes
		light.draw();
		table.draw();
		book.draw(objectShader);
		headphones.draw(objectShader);
		pen.draw(objectShader);
		cup.draw(objectShader);

		// Swaps front and back buffer
		glfwSwapBuffers(window);

		// -------------------- HANDLE INPUT --------------------
		glfwPollEvents();

		// Sleep
		//std::this_thread::sleep_for(std::chrono::milliseconds(12));
	}

	// Frees allocated resources used by GLFW
	glfwTerminate();
	return 0;
}
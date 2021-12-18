/*
* mesh.h
* This file contains declarations for mesh objects
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

#pragma once
#include <GLAD/glad.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders.h"
#include "stb_image.h"

namespace mesh
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture;
		Vertex() 
		{
			position = glm::vec3(0.0f, 0.0f, 0.0f);
			normal = glm::vec3(0.0f, 0.0f, 0.0f);
			texture = glm::vec2(0.0f, 0.0f);
		}
		// contructor with initializer list
		Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 t) : 
			position{ p }, normal{ n }, texture{ t } {}
	};

	struct Texture {
		GLuint id;
		std::string type;
		std::string path;
		Texture() : id{ 0 }, type{ "" }, path{ "" } {}
	};

	class TriangleMesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<GLushort> indices;
		glm::mat4 model;
		shaders::Shader shaderProgram;
		GLuint VAO, VBO, EBO, texture;
		glm::vec4 diffuse, specular;
		GLfloat shininess;
		std::string imagePath;
		TriangleMesh(std::vector<Vertex> uVertices, 
			std::vector<GLushort> uIndices,
			shaders::Shader uShader,
			std::string image,
			glm::vec4 uSpecular,
			GLfloat uShininess);
		TriangleMesh(std::vector<GLfloat> uVertices,
			std::vector<GLushort> uIndices,
			shaders::Shader uShader,
			std::string image,
			glm::vec4 uSpecular,
			GLfloat uShininess);
		TriangleMesh(const TriangleMesh &original);
		void draw();
		void rotate(GLfloat degrees, GLchar axis);
		void scale(GLfloat x, GLfloat y, GLfloat z);
		void translate(GLfloat x, GLfloat y, GLfloat z);
		
	private:
		void createMesh();
	};

	class Mesh
	{
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;

		Mesh(std::vector<Vertex> uVertices, std::vector<GLuint> uIndices, std::vector<Texture> uTextures);
		void draw(shaders::Shader& shader);

	private:
		GLuint VAO, VBO, EBO;
		void setup();
	};
}

#pragma once

#include <iostream>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "stb_image.h"

#include "shaders.h"
#include "mesh.h"

namespace model
{
	class Model
	{
	public:
		glm::mat4 model;
		std::vector<mesh::Texture> textures_loaded;
		Model(const char* path) {
			this->model = glm::mat4(1.0f);
			loadModel(path);
		}
		void rotate(GLfloat degrees, GLchar axis);
		void scale(GLfloat x, GLfloat y, GLfloat z);
		void translate(GLfloat x, GLfloat y, GLfloat z);
		void draw(shaders::Shader shader);
	private:
		std::vector<mesh::Mesh> meshes;
		std::string directory;
	
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		mesh::Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};
}

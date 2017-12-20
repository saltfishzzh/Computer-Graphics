#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SOIL2/SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cmath>

#include "Mesh.h"
#include "shader_m.h"

GLint TextureFromFile(const char* path, string directory);

class Model1
{
public:
	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Model1(const GLchar* path, const GLchar* vertexPath, const GLchar* fragmentPath, GLfloat width, GLfloat length) :
		modelShader(vertexPath, fragmentPath),
		translate_matrix(glm::mat4(1.0f)),
		rotate_matrix(glm::mat4(1.0f)),
		scale_matrix(glm::mat4(1.0f)),

		width(width),
		length(length)
	{
		this->loadModel(path);
	}

	// Draws the model and all its meshes
	void Draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
	{
		modelShader.use();
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);
		modelShader.setMat4("model", model);
		for (GLuint i = 0; i < this->meshes.size(); i++) {
			this->meshes[i].Draw(modelShader);
		}

	}

	void Draw(glm::mat4& view, glm::mat4& projection) {
		glm::mat4 model = translate_matrix*rotate_matrix*scale_matrix;
		modelShader.use();
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);
		modelShader.setMat4("model", model);
		for (GLuint i = 0; i < this->meshes.size(); i++) {
			this->meshes[i].Draw(modelShader);
		}
	}

	bool check_collision(glm::vec3 position, GLfloat r) {
		glm::vec3 myposition = translate_matrix*glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		GLfloat distance = std::pow(std::pow(position.x - myposition.x, 2) + std::pow(position.z - myposition.z, 2), 0.5);
		GLfloat radius_sum = r + (width + length) / 2 * 0.7;
		if (distance < radius_sum) {
			/*std::cout << "distance: " << distance << std::endl;
			std::cout << "radius_sum :  " << radius_sum << std::endl;
			std::cout << r << std::endl;
			std::cout << (width + length) / 2 * 0.7 << std::endl;*/
			return true;
		}
		else {
			return false;
		}
	}

	GLfloat get_width() {
		return width;
	}

	GLfloat get_length() {
		return length;
	}

	glm::mat4 translate_matrix;
	glm::mat4 rotate_matrix;
	glm::mat4 scale_matrix;
	glm::vec3 relative_vec;

	glm::vec3 getPosition() {
		return glm::vec3(translate_matrix[3]);
	}

private:
	/*  Model Data  */
	Shader modelShader;
	vector<Mesh> meshes;
	string directory;

	GLfloat width;
	GLfloat length;


	vector<Texture> textures_loaded;	// Stores all the textures

	void loadModel(string path)
	{
		// Read file
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		this->directory = path.substr(0, path.find_last_of('/'));
		this->processNode(scene->mRootNode, scene);
	}

	void processNode(aiNode* node, const aiScene* scene)
	{
		// Process each mesh
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			this->meshes.push_back(this->processMesh(mesh, scene));
		}
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			this->processNode(node->mChildren[i], scene);
		}

	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Data to fill
		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures;

		// go through each vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; 
			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			vertices.push_back(vertex);
		}
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			// 1. Diffuse maps
			vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular maps
			vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			GLboolean skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; 
					break;
				}
			}
			if (!skip)
			{   
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				this->textures_loaded.push_back(texture);
			}
		}
		return textures;
	}
};




GLint TextureFromFile(const char* path, string directory)
{
	//Generate texture ID and load texture data 
	string filename = string(path);
	filename = directory + '/' + filename;
	GLuint textureID;
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return textureID;
}
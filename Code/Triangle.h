#pragma once
#include <GL/glew.h>
#include <iostream>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader_m.h"
#include <GLFW/glfw3.h>
#include "SOIL2/SOIL2.h"

GLuint loadtexture_tri(char const * path) {
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	if (data) {
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		

		SOIL_free_image_data(data);
	} else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		SOIL_free_image_data(data);
	}

	return textureID;
}

class Triangle {
public:
	Triangle(std::array<GLfloat, 15> Vertices, const GLchar* vertexPath, const GLchar* fragmentPath) :
		vertices(Vertices),
		_shader(vertexPath, fragmentPath),
		_texture(loadtexture_tri("resource/rock.png"))
	{
		
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		// Position attribute
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		
		// vertex attribute
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		


		_shader.use();
		_shader.setInt("texture_diffuse1", 0);
		glBindVertexArray(0); // Unbind VAO
	}


	void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
		_shader.use();
		_shader.setMat4("projection", projection);
		_shader.setMat4("view", view);
		_shader.setMat4("model", model);


		//_shader.use();
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

private:
	Shader _shader;
	std::array<GLfloat, 15> vertices;
	GLuint VBO, VAO;
	GLuint _texture;

		
};

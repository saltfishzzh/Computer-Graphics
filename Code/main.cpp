// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//using namespace glm;

#include "Triangle.h"
#include "Rectangle.h"
#include "Grid.h"
#include "Model.h"
#include "shader_m.h"
#include "camera.h"
#include "const.h"
#include "SOIL2/SOIL2.h"


#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

GLFWwindow* createWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
GLuint loadTexture(const char *path);
GLuint loadCubemap(vector<const GLchar*> faces);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

// camera
Camera camera(true, glm::vec3(-0.0f, 2.0f, 3.0f));
Camera camera2(true, glm::vec3(0.0f, 0.0f, 1.0f));
int camera_num = 1;

std::vector<Model1*> items;
std::vector<Model1*> items_not_through;

float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(-2.0f, 10.0f, -1.0f);

GLfloat flag = 1;
GLfloat hp_scale = 0.1f;
GLfloat hungry_scale = 0.1f;

bool eating = false;

int main() {
	GLFWwindow* window = createWindow();

	// build and compile shaders
	// -------------------------
	
	
	// the terrain
	Grid g("source/shader.vs", "source/shader.fs");

	Model1 horse("resource/horse.obj", "source/shader.vs", "source/shader.fs", 1.0f, 2.5f);
	horse.translate_matrix = glm::translate(glm::vec3(0.0f, -2.0f, 0.0f));
	horse.rotate_matrix = glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, 0.0f, -1.0f));
	horse.rotate_matrix = glm::rotate(horse.rotate_matrix, glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
	horse.scale_matrix = glm::scale(glm::vec3(0.009f, 0.009f, 0.009f));
	camera.bind(&horse);

	
	Model1 grass_0("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_0.translate_matrix = glm::translate(glm::vec3(-72.55f, 10 * g.get_height(-72.55f, -81.6f), -81.6f));
	grass_0.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_0);

	Model1 blueflower_0("resource/objects/blueflower/blueflower.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower_0.translate_matrix = glm::translate(glm::vec3(7.28f, 10 * g.get_height(7.28f, -12.37f), -12.37f));
	blueflower_0.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower_0);

	Model1 bush2_0("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_0.translate_matrix = glm::translate(glm::vec3(22.2f, 10 * g.get_height(22.2f, -49.41f), -49.41f));
	bush2_0.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_0);

	Model1 bush2_1("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_1.translate_matrix = glm::translate(glm::vec3(122.74f, 10 * g.get_height(122.74f, -39.62f), -39.62f));
	bush2_1.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_1);

	Model1 blueflower2_0("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_0.translate_matrix = glm::translate(glm::vec3(155.41f, 10 * g.get_height(155.41f, 8.23f), 8.23f));
	blueflower2_0.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_0);

	Model1 grass_1("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_1.translate_matrix = glm::translate(glm::vec3(190.48f, 10 * g.get_height(190.48f, -81.96f), -81.96f));
	grass_1.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_1);

	Model1 bush2_2("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_2.translate_matrix = glm::translate(glm::vec3(-193.27f, 10 * g.get_height(-193.27f, -70.33f), -70.33f));
	bush2_2.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_2);

	Model1 blueflower_1("resource/objects/blueflower/blueflower.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower_1.translate_matrix = glm::translate(glm::vec3(5.88f, 10 * g.get_height(5.88f, 88.1f), 88.1f));
	blueflower_1.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower_1);

	Model1 grass_2("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_2.translate_matrix = glm::translate(glm::vec3(-94.03f, 10 * g.get_height(-94.03f, -43.32f), -43.32f));
	grass_2.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_2);

	Model1 bush_0("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_0.translate_matrix = glm::translate(glm::vec3(110.6f, 10 * g.get_height(110.6f, 39.85f), 39.85f));
	bush_0.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_0);

	Model1 bush3_0("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_0.translate_matrix = glm::translate(glm::vec3(-188.78f, 10 * g.get_height(-188.78f, 34.26f), 34.26f));
	bush3_0.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_0);

	Model1 bush2_3("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_3.translate_matrix = glm::translate(glm::vec3(-159.67f, 10 * g.get_height(-159.67f, 70.88f), 70.88f));
	bush2_3.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_3);

	Model1 blueflower2_1("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_1.translate_matrix = glm::translate(glm::vec3(-105.26f, 10 * g.get_height(-105.26f, -54.76f), -54.76f));
	blueflower2_1.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_1);

	Model1 bush_1("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_1.translate_matrix = glm::translate(glm::vec3(148.52f, 10 * g.get_height(148.52f, -13.17f), -13.17f));
	bush_1.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_1);

	Model1 blueflower2_2("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_2.translate_matrix = glm::translate(glm::vec3(-140.65f, 10 * g.get_height(-140.65f, -94.59f), -94.59f));
	blueflower2_2.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_2);

	Model1 blueflower_2("resource/objects/blueflower/blueflower.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower_2.translate_matrix = glm::translate(glm::vec3(99.72f, 10 * g.get_height(99.72f, 17.38f), 17.38f));
	blueflower_2.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower_2);

	Model1 bush2_4("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_4.translate_matrix = glm::translate(glm::vec3(-134.08f, 10 * g.get_height(-134.08f, 25.04f), 25.04f));
	bush2_4.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_4);

	Model1 bush_2("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_2.translate_matrix = glm::translate(glm::vec3(-131.97f, 10 * g.get_height(-131.97f, -73.22f), -73.22f));
	bush_2.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_2);

	Model1 bush_3("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_3.translate_matrix = glm::translate(glm::vec3(114.4f, 10 * g.get_height(114.4f, 5.34f), 5.34f));
	bush_3.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_3);

	Model1 blueflower2_3("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_3.translate_matrix = glm::translate(glm::vec3(-177.74f, 10 * g.get_height(-177.74f, -98.79f), -98.79f));
	blueflower2_3.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_3);

	Model1 blueflower2_4("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_4.translate_matrix = glm::translate(glm::vec3(-148.55f, 10 * g.get_height(-148.55f, 73.45f), 73.45f));
	blueflower2_4.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_4);

	Model1 bush2_5("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_5.translate_matrix = glm::translate(glm::vec3(103.57f, 10 * g.get_height(103.57f, 39.59f), 39.59f));
	bush2_5.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_5);

	Model1 blueflower2_5("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_5.translate_matrix = glm::translate(glm::vec3(-46.67f, 10 * g.get_height(-46.67f, -62.67f), -62.67f));
	blueflower2_5.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_5);

	Model1 bush3_1("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_1.translate_matrix = glm::translate(glm::vec3(-93.85f, 10 * g.get_height(-93.85f, 19.15f), 19.15f));
	bush3_1.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_1);

	Model1 blueflower_3("resource/objects/blueflower/blueflower.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower_3.translate_matrix = glm::translate(glm::vec3(69.47f, 10 * g.get_height(69.47f, 5.37f), 5.37f));
	blueflower_3.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower_3);

	Model1 blueflower_4("resource/objects/blueflower/blueflower.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower_4.translate_matrix = glm::translate(glm::vec3(-103.45f, 10 * g.get_height(-103.45f, -15.54f), -15.54f));
	blueflower_4.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower_4);

	Model1 bush2_6("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_6.translate_matrix = glm::translate(glm::vec3(194.26f, 10 * g.get_height(194.26f, -77.42f), -77.42f));
	bush2_6.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_6);

	Model1 bush3_2("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_2.translate_matrix = glm::translate(glm::vec3(-95.0f, 10 * g.get_height(-95.0f, -93.48f), -93.48f));
	bush3_2.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_2);

	Model1 grass_3("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_3.translate_matrix = glm::translate(glm::vec3(-19.41f, 10 * g.get_height(-19.41f, 58.87f), 58.87f));
	grass_3.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_3);

	Model1 blueflower2_6("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_6.translate_matrix = glm::translate(glm::vec3(34.63f, 10 * g.get_height(34.63f, 18.93f), 18.93f));
	blueflower2_6.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_6);

	Model1 bush2_7("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_7.translate_matrix = glm::translate(glm::vec3(-162.79f, 10 * g.get_height(-162.79f, 50.53f), 50.53f));
	bush2_7.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_7);

	Model1 blueflower2_7("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_7.translate_matrix = glm::translate(glm::vec3(153.26f, 10 * g.get_height(153.26f, 53.94f), 53.94f));
	blueflower2_7.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_7);

	Model1 bush_4("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_4.translate_matrix = glm::translate(glm::vec3(151.36f, 10 * g.get_height(151.36f, 77.97f), 77.97f));
	bush_4.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_4);

	Model1 bush_5("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_5.translate_matrix = glm::translate(glm::vec3(95.48f, 10 * g.get_height(95.48f, -61.7f), -61.7f));
	bush_5.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_5);

	Model1 bush3_3("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_3.translate_matrix = glm::translate(glm::vec3(-60.07f, 10 * g.get_height(-60.07f, 76.93f), 76.93f));
	bush3_3.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_3);

	Model1 bush_6("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_6.translate_matrix = glm::translate(glm::vec3(-78.06f, 10 * g.get_height(-78.06f, 20.45f), 20.45f));
	bush_6.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_6);

	Model1 blueflower2_8("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_8.translate_matrix = glm::translate(glm::vec3(115.97f, 10 * g.get_height(115.97f, 89.28f), 89.28f));
	blueflower2_8.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_8);

	Model1 blueflower_5("resource/objects/blueflower/blueflower.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower_5.translate_matrix = glm::translate(glm::vec3(175.88f, 10 * g.get_height(175.88f, 85.68f), 85.68f));
	blueflower_5.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower_5);

	Model1 bush_7("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_7.translate_matrix = glm::translate(glm::vec3(21.38f, 10 * g.get_height(21.38f, 4.07f), 4.07f));
	bush_7.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_7);

	Model1 bush_8("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_8.translate_matrix = glm::translate(glm::vec3(-70.13f, 10 * g.get_height(-70.13f, -86.44f), -86.44f));
	bush_8.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_8);

	Model1 bush3_4("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_4.translate_matrix = glm::translate(glm::vec3(51.52f, 10 * g.get_height(51.52f, 20.61f), 20.61f));
	bush3_4.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_4);

	Model1 bush_9("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_9.translate_matrix = glm::translate(glm::vec3(35.73f, 10 * g.get_height(35.73f, 58.61f), 58.61f));
	bush_9.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_9);

	Model1 bush3_5("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_5.translate_matrix = glm::translate(glm::vec3(-111.28f, 10 * g.get_height(-111.28f, -19.84f), -19.84f));
	bush3_5.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_5);

	Model1 bush2_8("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_8.translate_matrix = glm::translate(glm::vec3(-44.84f, 10 * g.get_height(-44.84f, 81.1f), 81.1f));
	bush2_8.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_8);

	Model1 bush_10("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_10.translate_matrix = glm::translate(glm::vec3(-125.96f, 10 * g.get_height(-125.96f, 94.68f), 94.68f));
	bush_10.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_10);

	Model1 grass_4("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_4.translate_matrix = glm::translate(glm::vec3(22.2f, 10 * g.get_height(22.2f, -42.12f), -42.12f));
	grass_4.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_4);

	Model1 blueflower_6("resource/objects/blueflower/blueflower.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower_6.translate_matrix = glm::translate(glm::vec3(-103.09f, 10 * g.get_height(-103.09f, 1.55f), 1.55f));
	blueflower_6.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower_6);

	Model1 bush2_9("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_9.translate_matrix = glm::translate(glm::vec3(-55.02f, 10 * g.get_height(-55.02f, 24.4f), 24.4f));
	bush2_9.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_9);

	Model1 grass_5("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_5.translate_matrix = glm::translate(glm::vec3(169.66f, 10 * g.get_height(169.66f, 50.63f), 50.63f));
	grass_5.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_5);

	Model1 bush3_6("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_6.translate_matrix = glm::translate(glm::vec3(-183.52f, 10 * g.get_height(-183.52f, -7.35f), -7.35f));
	bush3_6.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_6);

	Model1 bush3_7("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_7.translate_matrix = glm::translate(glm::vec3(159.11f, 10 * g.get_height(159.11f, -93.99f), -93.99f));
	bush3_7.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_7);

	Model1 bush_11("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_11.translate_matrix = glm::translate(glm::vec3(-147.71f, 10 * g.get_height(-147.71f, 37.91f), 37.91f));
	bush_11.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_11);

	Model1 bush3_8("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_8.translate_matrix = glm::translate(glm::vec3(117.38f, 10 * g.get_height(117.38f, 68.77f), 68.77f));
	bush3_8.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_8);

	Model1 bush3_9("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_9.translate_matrix = glm::translate(glm::vec3(89.75f, 10 * g.get_height(89.75f, -83.12f), -83.12f));
	bush3_9.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_9);

	Model1 grass_6("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_6.translate_matrix = glm::translate(glm::vec3(-162.1f, 10 * g.get_height(-162.1f, 23.01f), 23.01f));
	grass_6.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_6);

	Model1 blueflower2_9("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_9.translate_matrix = glm::translate(glm::vec3(-133.61f, 10 * g.get_height(-133.61f, -12.59f), -12.59f));
	blueflower2_9.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_9);

	Model1 bush3_10("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_10.translate_matrix = glm::translate(glm::vec3(-9.77f, 10 * g.get_height(-9.77f, -66.48f), -66.48f));
	bush3_10.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_10);

	Model1 grass_7("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_7.translate_matrix = glm::translate(glm::vec3(58.28f, 10 * g.get_height(58.28f, -17.39f), -17.39f));
	grass_7.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_7);

	Model1 blueflower_7("resource/objects/blueflower/blueflower.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower_7.translate_matrix = glm::translate(glm::vec3(-127.25f, 10 * g.get_height(-127.25f, -73.33f), -73.33f));
	blueflower_7.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower_7);

	Model1 blueflower2_10("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_10.translate_matrix = glm::translate(glm::vec3(-161.95f, 10 * g.get_height(-161.95f, 23.33f), 23.33f));
	blueflower2_10.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_10);

	Model1 bush3_11("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_11.translate_matrix = glm::translate(glm::vec3(193.62f, 10 * g.get_height(193.62f, 59.48f), 59.48f));
	bush3_11.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_11);

	Model1 bush3_12("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_12.translate_matrix = glm::translate(glm::vec3(44.59f, 10 * g.get_height(44.59f, 92.06f), 92.06f));
	bush3_12.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_12);

	Model1 grass_8("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_8.translate_matrix = glm::translate(glm::vec3(-13.09f, 10 * g.get_height(-13.09f, 16.14f), 16.14f));
	grass_8.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_8);

	Model1 bush_12("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_12.translate_matrix = glm::translate(glm::vec3(183.74f, 10 * g.get_height(183.74f, 80.26f), 80.26f));
	bush_12.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_12);

	Model1 bush2_10("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_10.translate_matrix = glm::translate(glm::vec3(-189.33f, 10 * g.get_height(-189.33f, -59.82f), -59.82f));
	bush2_10.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_10);

	Model1 bush2_11("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_11.translate_matrix = glm::translate(glm::vec3(82.18f, 10 * g.get_height(82.18f, 3.51f), 3.51f));
	bush2_11.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_11);

	Model1 bush2_12("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_12.translate_matrix = glm::translate(glm::vec3(120.23f, 10 * g.get_height(120.23f, -79.54f), -79.54f));
	bush2_12.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_12);

	Model1 bush_13("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_13.translate_matrix = glm::translate(glm::vec3(-110.85f, 10 * g.get_height(-110.85f, -92.61f), -92.61f));
	bush_13.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_13);

	Model1 bush_14("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_14.translate_matrix = glm::translate(glm::vec3(-143.63f, 10 * g.get_height(-143.63f, -83.72f), -83.72f));
	bush_14.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_14);

	Model1 bush2_13("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_13.translate_matrix = glm::translate(glm::vec3(-187.55f, 10 * g.get_height(-187.55f, 16.51f), 16.51f));
	bush2_13.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_13);

	Model1 bush_15("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_15.translate_matrix = glm::translate(glm::vec3(-27.7f, 10 * g.get_height(-27.7f, -34.94f), -34.94f));
	bush_15.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_15);

	Model1 blueflower2_11("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_11.translate_matrix = glm::translate(glm::vec3(-152.56f, 10 * g.get_height(-152.56f, -8.31f), -8.31f));
	blueflower2_11.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_11);

	Model1 grass_9("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_9.translate_matrix = glm::translate(glm::vec3(-184.63f, 10 * g.get_height(-184.63f, -9.58f), -9.58f));
	grass_9.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_9);

	Model1 grass_10("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_10.translate_matrix = glm::translate(glm::vec3(-143.1f, 10 * g.get_height(-143.1f, 77.48f), 77.48f));
	grass_10.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_10);

	Model1 grass_11("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_11.translate_matrix = glm::translate(glm::vec3(-69.51f, 10 * g.get_height(-69.51f, 33.24f), 33.24f));
	grass_11.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_11);

	Model1 bush_16("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_16.translate_matrix = glm::translate(glm::vec3(-108.86f, 10 * g.get_height(-108.86f, 73.62f), 73.62f));
	bush_16.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_16);

	Model1 blueflower_8("resource/objects/blueflower/blueflower.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower_8.translate_matrix = glm::translate(glm::vec3(-144.33f, 10 * g.get_height(-144.33f, 56.08f), 56.08f));
	blueflower_8.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower_8);

	Model1 bush3_13("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_13.translate_matrix = glm::translate(glm::vec3(190.19f, 10 * g.get_height(190.19f, 32.32f), 32.32f));
	bush3_13.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_13);

	Model1 bush_17("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_17.translate_matrix = glm::translate(glm::vec3(-196.71f, 10 * g.get_height(-196.71f, 68.67f), 68.67f));
	bush_17.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_17);

	Model1 blueflower2_12("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_12.translate_matrix = glm::translate(glm::vec3(121.57f, 10 * g.get_height(121.57f, 17.49f), 17.49f));
	blueflower2_12.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_12);

	Model1 grass_12("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_12.translate_matrix = glm::translate(glm::vec3(-49.02f, 10 * g.get_height(-49.02f, 15.75f), 15.75f));
	grass_12.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_12);

	Model1 bush2_14("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_14.translate_matrix = glm::translate(glm::vec3(72.75f, 10 * g.get_height(72.75f, 91.08f), 91.08f));
	bush2_14.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_14);

	Model1 bush_18("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_18.translate_matrix = glm::translate(glm::vec3(148.65f, 10 * g.get_height(148.65f, 89.67f), 89.67f));
	bush_18.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_18);

	Model1 blueflower2_13("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_13.translate_matrix = glm::translate(glm::vec3(-161.38f, 10 * g.get_height(-161.38f, 82.66f), 82.66f));
	blueflower2_13.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_13);

	Model1 grass_13("resource/objects/grass/grass.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	grass_13.translate_matrix = glm::translate(glm::vec3(62.97f, 10 * g.get_height(62.97f, -5.04f), -5.04f));
	grass_13.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&grass_13);

	Model1 bush2_15("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_15.translate_matrix = glm::translate(glm::vec3(-57.63f, 10 * g.get_height(-57.63f, -59.91f), -59.91f));
	bush2_15.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_15);

	Model1 bush3_14("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_14.translate_matrix = glm::translate(glm::vec3(13.56f, 10 * g.get_height(13.56f, -85.77f), -85.77f));
	bush3_14.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_14);

	Model1 blueflower_9("resource/objects/blueflower/blueflower.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower_9.translate_matrix = glm::translate(glm::vec3(166.24f, 10 * g.get_height(166.24f, -83.0f), -83.0f));
	blueflower_9.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower_9);

	Model1 blueflower2_14("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_14.translate_matrix = glm::translate(glm::vec3(189.35f, 10 * g.get_height(189.35f, 54.22f), 54.22f));
	blueflower2_14.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_14);

	Model1 blueflower_10("resource/objects/blueflower/blueflower.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower_10.translate_matrix = glm::translate(glm::vec3(-109.26f, 10 * g.get_height(-109.26f, -95.07f), -95.07f));
	blueflower_10.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower_10);

	Model1 bush2_16("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_16.translate_matrix = glm::translate(glm::vec3(-42.04f, 10 * g.get_height(-42.04f, -39.74f), -39.74f));
	bush2_16.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_16);

	Model1 bush3_15("resource/objects/bush3/bush3.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush3_15.translate_matrix = glm::translate(glm::vec3(-81.69f, 10 * g.get_height(-81.69f, 3.19f), 3.19f));
	bush3_15.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush3_15);

	Model1 blueflower2_15("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_15.translate_matrix = glm::translate(glm::vec3(108.26f, 10 * g.get_height(108.26f, 40.56f), 40.56f));
	blueflower2_15.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_15);

	Model1 bush2_17("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_17.translate_matrix = glm::translate(glm::vec3(-5.29f, 10 * g.get_height(-5.29f, 55.93f), 55.93f));
	bush2_17.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_17);

	Model1 bush2_18("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_18.translate_matrix = glm::translate(glm::vec3(72.34f, 10 * g.get_height(72.34f, 99.86f), 99.86f));
	bush2_18.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_18);

	Model1 bush2_19("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_19.translate_matrix = glm::translate(glm::vec3(-25.34f, 10 * g.get_height(-25.34f, -58.51f), -58.51f));
	bush2_19.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_19);

	Model1 bush_19("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_19.translate_matrix = glm::translate(glm::vec3(-7.3f, 10 * g.get_height(-7.3f, 82.48f), 82.48f));
	bush_19.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_19);

	Model1 bush2_20("resource/objects/bush2/bush2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush2_20.translate_matrix = glm::translate(glm::vec3(-46.7f, 10 * g.get_height(-46.7f, 7.34f), 7.34f));
	bush2_20.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush2_20);

	Model1 blueflower2_16("resource/objects/blueflower2/blueflower2.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	blueflower2_16.translate_matrix = glm::translate(glm::vec3(-183.61f, 10 * g.get_height(-183.61f, 20.23f), 20.23f));
	blueflower2_16.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&blueflower2_16);

	Model1 bush_20("resource/objects/bush/bush.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	bush_20.translate_matrix = glm::translate(glm::vec3(159.7f, 10 * g.get_height(159.7f, -93.43f), -93.43f));
	bush_20.scale_matrix = glm::scale(glm::vec3(0.01f, 0.01f, 0.01f));
	items.push_back(&bush_20);

	

	Model1 rock_0("resource/objects/rock/rock.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	rock_0.translate_matrix = glm::translate(glm::vec3(26.0f, 10 * g.get_height(26.0f, -39.21f), -39.21f));
	rock_0.scale_matrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	items_not_through.push_back(&rock_0);

	Model1 rock_1("resource/objects/rock/rock.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	rock_1.translate_matrix = glm::translate(glm::vec3(30.74f, 10 * g.get_height(30.74f, 88.99f), 88.99f));
	rock_1.scale_matrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	items_not_through.push_back(&rock_1);

	Model1 rock_2("resource/objects/rock/rock.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	rock_2.translate_matrix = glm::translate(glm::vec3(-178.43f, 10 * g.get_height(-178.43f, 75.76f), 75.76f));
	rock_2.scale_matrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	items_not_through.push_back(&rock_2);

	Model1 rock_3("resource/objects/rock/rock.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	rock_3.translate_matrix = glm::translate(glm::vec3(153.16f, 10 * g.get_height(153.16f, -56.69f), -56.69f));
	rock_3.scale_matrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	items_not_through.push_back(&rock_3);

	Model1 rock_4("resource/objects/rock/rock.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	rock_4.translate_matrix = glm::translate(glm::vec3(-152.5f, 10 * g.get_height(-152.5f, -61.23f), -61.23f));
	rock_4.scale_matrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	items_not_through.push_back(&rock_4);

	Model1 rock_5("resource/objects/rock/rock.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	rock_5.translate_matrix = glm::translate(glm::vec3(162.88f, 10 * g.get_height(162.88f, 85.91f), 85.91f));
	rock_5.scale_matrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	items_not_through.push_back(&rock_5);

	Model1 rock_6("resource/objects/rock/rock.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	rock_6.translate_matrix = glm::translate(glm::vec3(47.35f, 10 * g.get_height(47.35f, 56.9f), 56.9f));
	rock_6.scale_matrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	items_not_through.push_back(&rock_6);

	Model1 rock_7("resource/objects/rock/rock.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	rock_7.translate_matrix = glm::translate(glm::vec3(72.28f, 10 * g.get_height(72.28f, 54.66f), 54.66f));
	rock_7.scale_matrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	items_not_through.push_back(&rock_7);

	Model1 rock_8("resource/objects/rock/rock.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	rock_8.translate_matrix = glm::translate(glm::vec3(-115.01f, 10 * g.get_height(-115.01f, 14.42f), 14.42f));
	rock_8.scale_matrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	items_not_through.push_back(&rock_8);

	Model1 rock_9("resource/objects/rock/rock.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	rock_9.translate_matrix = glm::translate(glm::vec3(-101.98f, 10 * g.get_height(-101.98f, -69.75f), -69.75f));
	rock_9.scale_matrix = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	items_not_through.push_back(&rock_9);


	Model1 creeper_green("resource/objects/Creeper/CreeperGreen.obj", "source/shader.vs", "source/shader.fs", 3.5f, 4.0f);
	glm::vec3 monster_green_position = glm::vec3(0.0f, 0.5f, -5.0f);
	creeper_green.translate_matrix = glm::translate(monster_green_position);
	creeper_green.scale_matrix = glm::scale(glm::vec3(0.002f, 0.002f, 0.002f));
	items_not_through.push_back(&creeper_green);

	Model1 creeper_red("resource/objects/Creeper/CreeperRed.obj", "source/shader.vs", "source/shader.fs", 3.5f, 4.0f);
	glm::vec3 monster_red_position = glm::vec3(7.0f, 0.5f, 11.0f);
	creeper_red.translate_matrix = glm::translate(monster_red_position);
	creeper_red.scale_matrix = glm::scale(glm::vec3(0.002f, 0.002f, 0.002f));
	items_not_through.push_back(&creeper_red);

	Model1 creeper_black("resource/objects/Creeper/CreeperBlack.obj", "source/shader.vs", "source/shader.fs", 3.5f, 4.0f);
	glm::vec3 monster_black_position = glm::vec3(-21.0f, 0.5f, 21.0f);
	creeper_black.translate_matrix = glm::translate(monster_black_position);
	creeper_black.scale_matrix = glm::scale(glm::vec3(0.002f, 0.002f, 0.002f));
	items_not_through.push_back(&creeper_black);

	Model1 sword("resource/sword.obj", "source/shader.vs", "source/shader.fs", 1.0f, 1.0f);
	sword.translate_matrix = glm::translate(glm::vec3(0.25f, 0.0f, -0.7f));
	sword.rotate_matrix = glm::rotate(glm::pi<float>() / 2, glm::vec3(0.0f, -1.0f, -0.0f));
	sword.scale_matrix = glm::scale(glm::vec3(0.05f, 0.05f, -0.05f));
	camera.bind(&sword);

	



	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// first, configure the cube's VAO (and VBO)
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	vector<const GLchar*> faces
	{
		"resource/RIGHT.jpg",
		"resource/LEFT.jpg",
		"resource/TOP.jpg",
		"resource/BOTTOM.jpg",
		"resource/BACK.jpg",
		"resource/FRONT.jpg"
	};
	GLuint cubemapTexture = loadCubemap(faces);

	GLuint diffuseMap = loadTexture("resource/marble.jpg");

	// shader configuration
	// --------------------

	Shader skyboxShader("source/skybox.vs", "source/skybox.fs");

	Shader lightingShader_box("source/lighting_maps.vs", "source/lighting_maps.fs");
	Shader lampShader("source/lamp.vs", "source/lamp.fs");

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	lightingShader_box.use();
	lightingShader_box.setInt("material.diffuse", 0);

	//hp
	std::array <GLfloat, 20> vertices{
		// Positions         // 
		-1.0f, 0.3f, 0.0f,   1.0f, 0.0f,  // Bottom Right
		-1.0f, -0.3f, 0.0f,  0.0f, 0.0f,  // Bottom Left
		1.0f,  -0.3f, 0.0f,   0.0f, 1.0f,   // Top 
		1.0f,  0.3f, 0.0f,   1.0f, 1.0f   // Top 
	};




	Rectangle h("resource/red.png", vertices, "source/shader.vs", "source/shader.fs");
	Rectangle h1("resource/orange.png", vertices, "source/shader.vs", "source/shader.fs");

	int move_counter1 = 0;
	int move_counter2 = 0;
	int move_counter3 = 0;

	glm::vec3 Position = camera.getPosition();

	glm::vec3 green_move_towards = glm::vec3(Position.x - monster_green_position.x, Position.y - monster_green_position.y, Position.z - monster_green_position.z);
	glm::vec3 red_move_towards = glm::vec3(Position.x - monster_red_position.x, Position.y - monster_red_position.y, Position.z - monster_red_position.z);
	glm::vec3 black_move_towards = glm::vec3(Position.x - monster_black_position.x, Position.y - monster_black_position.y, Position.z - monster_black_position.z);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {

		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat temp = g.get_height(camera.Position.x / 10.0f, camera.Position.z / 10.0f);
		camera.Position.y += temp * 10;

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader_box.use();
		lightingShader_box.setVec3("light.position", lightPos);
		lightingShader_box.setVec3("viewPos", camera.Position);

		// light properties
		lightingShader_box.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		lightingShader_box.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader_box.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		lightingShader_box.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader_box.setFloat("material.shininess", 64.0f);

		glm::mat4 projection_1 = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view_1 = camera.GetViewMatrix();

		lightingShader_box.setMat4("projection", projection_1);
		lightingShader_box.setMat4("view", view_1);

		// world transformation
		//glm::mat4 lightModel;
		glm::mat4 lightModel = glm::translate(glm::vec3(24.0f, g.get_height(24.0f, 12.0f)*10+1.0f, 12.0f));
		lightingShader_box.setMat4("model", lightModel);


		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);

		// render the cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// also draw the lamp object
		lampShader.use();
		lampShader.setMat4("projection", projection_1);
		lampShader.setMat4("view", view_1);
		glm::mat4 lampModel;
		lampModel = glm::translate(lampModel, lightPos);
		lampModel = glm::scale(lampModel, glm::vec3(0.2f)); // a smaller cube
		lampShader.setMat4("model", lampModel);

		//c.draw(model, view, projection);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

	


		/*glm::mat4 view_1 = camera.GetViewMatrix();
		glm::mat4 projection_1 = glm::perspective(camera.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);*/

		glm::mat4 view_2 = camera2.GetViewMatrix();
		glm::mat4 projection_2 = glm::perspective(camera2.Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100000.0f);



		glm::mat4 view;
		glm::mat4 projection;

		switch (camera_num) {
		case 1:
			view = view_1;
			projection = projection_1;
			break;
		case 2:
			view = view_2;
			projection = projection_2;
			break;
		default:
			view = view_1;
			projection = projection_1;
			break;
		}

		glm::vec3 reduce_1 = { hp_scale, 0.1f, 0.1f };
		glm::vec3 reduce_2 = { hungry_scale, 0.1f, 0.1f };

		glm::mat4 model_rec_h1 = glm::translate(glm::vec3(0.7f + hp_scale, 0.5f, 0.0f));
		model_rec_h1 = glm::scale(model_rec_h1, reduce_1);
		h.draw(model_rec_h1, view_2, projection_2);


		glm::mat4 model_rec_h2 = glm::translate(glm::vec3(0.7f + hungry_scale, 0.4f, 0.0f));
		model_rec_h2 = glm::scale(model_rec_h2, reduce_2);
		h1.draw(model_rec_h2, view_2, projection_2);


		glm::mat4 model(1.0f);
		glm::mat4 model_camera = glm::translate(camera.Position);
		camera.draw_bind_models(view, projection);
		//glm::mat4 model_horse = glm::translate(horse.transtlate_matrix, glm::vec3(1.0f, 1.0f, 1.0f))*horse.rotate_matrix*horse.scale_matrix;
		//horse.Draw(view, projection);
		//sword.Draw(view, projection);

		glm::mat4 ground_model = glm::scale(glm::vec3(10.0f, 10.0f, 10.0f));
		g.draw(ground_model, view, projection);



		//rock.Draw(view, projection);
		//blueflower.Draw(view, projection);
		//blueflower2.Draw(view, projection);
		//bush.Draw(view, projection);
		for (auto i = items.begin(); i != items.end(); i++) {
			(*i)->Draw(view, projection);
		}

		for (auto i = items_not_through.begin(); i != items_not_through.end(); i++) {
			if (i - items_not_through.begin() < 10) (*i)->Draw(view, projection);
		}

		creeper_green.Draw(view, projection);
		creeper_red.Draw(view, projection);
		creeper_black.Draw(view, projection);
		if (move_counter1 % 500 == 0) {
			move_counter1 = 0;
			Position = camera.getPosition();
			green_move_towards = glm::vec3(Position.x - monster_green_position.x, Position.y - monster_green_position.y, Position.z - monster_green_position.z);

			float angle1 = (float)3.141593 / 2 - atan(green_move_towards.z / green_move_towards.x);
			creeper_green.rotate_matrix = glm::rotate(green_move_towards.x > 0 ? angle1 : angle1 + (float)3.141593, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else {
			monster_green_position = glm::vec3(monster_green_position.x + green_move_towards.x / 500.0f, monster_green_position.y, monster_green_position.z + green_move_towards.z / 500.0f);
			creeper_green.translate_matrix = glm::translate(monster_green_position);
		}

	
		if (move_counter2 % 400 == 0) {
			move_counter2 = 0;
			Position = camera.getPosition();
			red_move_towards = glm::vec3(Position.x - monster_red_position.x, Position.y - monster_red_position.y, Position.z - monster_red_position.z);

			float angle2 = (float)3.141593 / 2 - atan(red_move_towards.z / red_move_towards.x);
			creeper_red.rotate_matrix = glm::rotate(red_move_towards.x > 0 ? angle2 : angle2 + (float)3.141593, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else {
			monster_red_position = glm::vec3(monster_red_position.x + red_move_towards.x / 500.0f, monster_red_position.y, monster_red_position.z + red_move_towards.z / 500.0f);
			creeper_red.translate_matrix = glm::translate(monster_red_position);
		}

		
		if (move_counter3 % 300 == 0) {
			move_counter3 = 0;
			Position = camera.getPosition();
			black_move_towards = glm::vec3(Position.x - monster_black_position.x, Position.y - monster_black_position.y, Position.z - monster_black_position.z);

			float angle3 = (float)3.141593 / 2 - atan(black_move_towards.z / black_move_towards.x);
			creeper_black.rotate_matrix = glm::rotate(black_move_towards.x > 0 ? angle3 : angle3 + (float)3.141593, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else {
			monster_black_position = glm::vec3(monster_black_position.x + black_move_towards.x / 500.0f, monster_black_position.y, monster_black_position.z + black_move_towards.z / 500.0f);
			creeper_black.translate_matrix = glm::translate(monster_black_position);
		}

		//draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		camera.Position.y -= temp*10;


		glDepthFunc(GL_LESS); // set depth function back to default

							  // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
							  // -------------------------------------------------------------------------------
		move_counter1++;
		move_counter2++;
		move_counter3++;
		if (move_counter1 % 100 == 0 && !eating) {
			if (hungry_scale > 0) hungry_scale = (hungry_scale - 0.0005f * flag) <= 0.1f ? hungry_scale - 0.0005f * flag : 0.1f;
			else hp_scale = (hp_scale - 0.0001 * flag) >= 0 ? hp_scale - 0.0001f * flag : 0;
		}
		if (hp_scale == 0.0f) break;
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resource once they've outlived their purpose:
	//glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVAO);

	glfwTerminate();
	return 0;

}

GLFWwindow* createWindow() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MineStarvation", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW winddow" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		//return EXIT_FAILURE;
		return NULL;
	}

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	return window;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
	static int count = 0;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	bool meet_monster = false;
	bool &meet = meet_monster;
	if (camera.check_collision(items_not_through, meet)) {
		if (meet && (count % 50 == 0)) {
			hp_scale = (hp_scale - 0.01f * flag) >= 0 ? hp_scale - 0.01f * flag : 0;
		}
		
//		std::cout << " collision! " << std::endl;
		camera.set_former();
		count++;
	}
	else if (camera.check_collision(items, meet)) {
		if (count % 100 == 0) {
			if (hungry_scale == 0.1f) hp_scale = (hp_scale + 0.01f * flag) <= 0.1f ? hp_scale + 0.01f * flag : 0.1f;
			else hungry_scale = (hungry_scale + 0.01f * flag) <= 0.1f ? hungry_scale + 0.01f * flag : 0.1f;
			eating = true;
		}
//		std::cout << " collision! " << std::endl;
		camera.set_former();
		count++;
	}
	else {
		count = 0;
		eating = false;
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
GLuint loadTexture(char const * path) {
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
	if (data) {

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SOIL_free_image_data(data);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		SOIL_free_image_data(data);
	}

	return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
GLuint loadCubemap(vector<const GLchar*> faces) {
	GLuint textureID;
	unsigned char *data;
	int width, height;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (GLuint i = 0; i < faces.size(); i++) {
		data = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		SOIL_free_image_data(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


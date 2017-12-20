#ifndef CAMERA_H
#define CAMERA_H

//#include "includes/glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Model.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;


class Camera {
public:
	// Camera Attributes
	glm::vec3 Position;

	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;



	// Constructor with vectors
	Camera(bool cannotup, glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) :
		Front(glm::vec3(0.0f, 2.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM),
		former_pos(position),
		Position(position),
		WorldUp(up),
		Yaw(yaw),
		Pitch(pitch),
		is_bind(true),
		cannotup(cannotup),
		rotation(glm::mat4(1.0f)) {

		updateCameraVectors();
	}
	
	Camera(bool cannotup, float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
		Front(glm::vec3(0.0f, 0.0f, -1.0f)),
		MovementSpeed(SPEED),
		MouseSensitivity(SENSITIVTY),
		Zoom(ZOOM),
		cannotup(cannotup) {
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}



	glm::vec3 getPosition() {
		return Position;
	}



	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	glm::vec3 GetFront() {
		return Front;
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime) {
		float velocity = MovementSpeed * deltaTime;
		glm::mat3 temp;
		if (cannotup) {
			temp = glm::mat3(1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f);
		}
		else {
			temp = glm::mat3(1.0f);
		}

		former_pos = Position;

		switch (direction) {
		case FORWARD:
			Position += temp*Front * velocity;
			translation = glm::translate(Front * velocity);
			break;
		case BACKWARD:
			Position -= temp*Front * velocity;
			translation = glm::translate(Front * (-velocity));
			break;
		case LEFT:
			Position -= temp*Right * velocity;
			translation = glm::translate(Right * (-velocity));
			break;
		case RIGHT:
			Position += temp*Right * velocity;
			translation = glm::translate(Right * velocity);
			break;
		default:
			break;
		}



	}

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		rotation = glm::rotate(rotation, glm::radians(xoffset), glm::vec3(0.0f, -1.0f, 0.0f));
		rotate_radians = glm::radians(xoffset);

		if (constrainPitch) {
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		updateCameraVectors();
	}

	void ProcessMouseScroll(float yoffset) {
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

	void bind(Model1* model) {
		if (bind_models.size() == 0)
		{
			model->relative_vec = glm::vec3(0);
		}
		else {
			model->relative_vec = -glm::vec3(bind_models[0]->getPosition() - model->getPosition());
		}

		bind_models.push_back(model);
		if (model->get_length()>length) {
			length = model->get_length();
		}

		if (model->get_width()>width) {
			width = model->get_width();
		}
	}



	void draw_bind_models(glm::mat4 view, glm::mat4 projection) {
		if (is_bind) {
			int m = 0;
			for (auto i = bind_models.begin(); i != bind_models.end(); i++) {
				glm::vec3 relative_translate = rotation*glm::vec4((*i)->relative_vec, 0);
				glm::mat4 model = glm::translate((bind_models[0])->translate_matrix, Position);
				model = glm::translate(model, relative_translate);
				model *= rotation;
				model *= (*i)->rotate_matrix;
				model *= (*i)->scale_matrix;
				model *= scale;
				(*i)->Draw(model, view, projection);




			}
		}
		else {
			for (auto i = bind_models.begin(); i != bind_models.end(); i++) {
				glm::mat4 model = (*i)->translate_matrix;
				model *= (*i)->rotate_matrix;
				model *= (*i)->scale_matrix;
				(*i)->Draw(model, view, projection);
			}
		}

	}

	void add_Position(glm::vec3 a) {
		Position += a;
	}

	void set_isbind(bool is_bind) {
		this->is_bind = is_bind;
	}

	void set_width_length(GLfloat w, GLfloat l) {
		width = w;
		length = l;
	}

	bool check_collision(std::vector<Model1*>& items, bool& meet_monster) {
		for (auto i = items.begin(); i != items.end(); i++) {
			if ((*i)->check_collision(Position, (width + length) / 2 * 0.7)) {
				if (((i - items.begin()) == 10) || ((i - items.begin()) == 11) || ((i - items.begin()) == 12)) meet_monster = true;
				return true;
			}
		}
		return false;
	}

	void set_former() {
		Position = former_pos;
	}

private:
	void updateCameraVectors() {
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));


		Front = glm::normalize(front);
		// Also calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}


	std::vector<Model1*> bind_models;

	glm::mat4 translation;
	glm::mat4 rotation;
	GLfloat rotate_radians;
	glm::mat4 scale;
	bool is_bind;
	bool cannotup;
	GLfloat width;
	GLfloat length;
	glm::vec3 former_pos;


};
#endif
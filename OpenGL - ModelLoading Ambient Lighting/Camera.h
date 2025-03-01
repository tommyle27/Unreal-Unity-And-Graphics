#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/matrix_transform.hpp>

// define several possible options for the camera movement, used as abstraction to stay away from window-system specific input methods
enum camera_movement {
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT
};


// defualt camera values
const float YAW			= -90.0f;
const float PITCH		= 0.0f;
const float SPEED		= 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM		= 55.0f;


// an abstract camera class that processes input and calculates euler angles vectors and matrices for use in OpenGL
class Camera {
public:
	// camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp; 

	// euler angles
	float Yaw;
	float Pitch;

	// camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// returns the view matrix calculated using euler angles and the lookat matrix
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	// processes input received from the keyboard like input system accepts parameter in the form of camera defined ENUM
	void ProcessKeyboard(camera_movement direction, float deltaTime) {
		float velocity = MovementSpeed * deltaTime;

		if (direction == FORWARD) {
			Position += Front * velocity;
		}
		if (direction == BACKWARD) {
			Position -= Front * velocity;
		}
		if (direction == RIGHT) {
			Position += Right * velocity;
		}
		if (direction == LEFT) {
			Position -= Right * velocity;
		}
	}

	// process input received from a mouse input system. expects the offset value in both x and y directions
	void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
		xOffset *= MouseSensitivity;
		yOffset *= MouseSensitivity;

		Yaw += xOffset;
		Pitch += yOffset;

		// make sure that when pitch is out of bounds screen doesn't flip
		if (constrainPitch) {
			if (Pitch > 89.0f) Pitch = 89.0f;
			if (Pitch < -89.0f) Pitch = -89.0f;
		}

		// update front right vectors yusing the updated euler angles
		updateCameraVectors();
	}


	// process input from mouse scroll wheel
	void ProcessMouseScroll(float yOffset) {
		Zoom -= (float)yOffset;

		if (Zoom < 1.0f) Zoom = 1.0f;
		if (Zoom > 55.0f) Zoom = 55.0f;
	}
	


private:
	// calc the front vector from the camera's updated eurler angles
	void updateCameraVectors() {
		// calculate the new front vector
		glm::vec3 front;

		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

		Front = glm::normalize(front);

		// re-calculate the right and up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));  
	}
};


#endif // !CAMERA_H



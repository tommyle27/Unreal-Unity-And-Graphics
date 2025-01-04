#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/matrix_transform.hpp>


// define list of camera movement
enum cameraMovement {
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT

};


// intialize camera varaibles
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float FOV = 65.0f;
const float SPEED = 5.0f;
const float SENSITIVY = 0.1f;

class Camera {
public:
	// camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler angles
	float Yaw;
	float Pitch;

	// Camera Options:
	float fov;
	float movementSpeed;
	float mouseSensitivity;

	// construct the camera
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVY), fov(FOV) {
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		// update camera vectors
		updateCameraVectors();
	}

	// returns the view matrix calcualted using euler angles and the lookAt matrix
	glm::mat4 getViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	// processes input received from any keyboard-like input system. accepts input paramter
	// in the form of camera defined ENUM to abstract it from windowing systems
	void keyboardInput(cameraMovement direction, float deltaTime) {
		float velocity = movementSpeed * deltaTime;

		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
	}

	// processes input received from a mouse input system. Expects the offset value 
	// in both the x and y direction
	void processMouseMovement(float xOffset, float yOffset, GLboolean constraintPitch = true) {
		xOffset *= mouseSensitivity;
		yOffset *= mouseSensitivity;

		Yaw += xOffset;
		Pitch += yOffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constraintPitch) {
			if (Pitch > 89.0f) Pitch = 89.0f;
			if (Pitch < -89.0f) Pitch = -89.0f;
		}

		// update Front, right and up vectors using the updated eular angles
		updateCameraVectors();
	}

	// processes input received from a mouse scroll-wheel event
	// Only requires input on the vertical wheel axis
	void processMouseScroll(float yOffset) {
		fov -= (float)yOffset;

		if (fov < 1.0f) fov = 1.0f;
		if (fov > 50.0f) fov = 50.0f;
	}

private:
	// calculates the front vector from the cameras updated euler angles
	void updateCameraVectors() {

		// calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

#endif
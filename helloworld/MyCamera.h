#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum CameraDirection
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float CAMERASPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
private:
	//camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;
	//eular angles
	float Yaw;
	float Pitch;
	//camera operation
	float CameraSpeed;
	float Sensitivity;
	

public:
	float Zoom;
	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)):
		Front(glm::vec3(0.0f, 0.0f, -1.0f)), Yaw(YAW),
		Pitch(PITCH), Zoom(ZOOM), Sensitivity(SENSITIVITY), CameraSpeed(CAMERASPEED)
	{
		Position = pos;
		WorldUp = up;
		UpdateCameraData();
	}
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ) :Front(glm::vec3(0.0f, 0.0f, -1.0f)), Yaw(YAW),
		Pitch(PITCH), Zoom(ZOOM), Sensitivity(SENSITIVITY), CameraSpeed(CAMERASPEED)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		UpdateCameraData();
	}

	glm::mat4 GetCameraMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	void ProcessInput(CameraDirection direction, float deltaTime)
	{
		float velocity = CameraSpeed * deltaTime;
		switch (direction)
		{
			case CameraDirection::FORWARD:
				Position += velocity * Front;
				break;
			case CameraDirection::BACKWARD:
				Position -= velocity * Front;
				break;
			case CameraDirection::LEFT:
				Position -= velocity * Right;
				break;
			case CameraDirection::RIGHT:
				Position += velocity * Right;
				break;
		}
	}

	void ProcessMouseInput(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= Sensitivity;
		yoffset *= Sensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		UpdateCameraData();
	}

	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

private:
	void UpdateCameraData()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
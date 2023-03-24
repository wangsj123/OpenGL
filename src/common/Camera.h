#pragma once
#include <glm/glm.hpp>

enum Camera_Movement {
	CM_FORWARD,
	CM_BACKWARD,
	CM_LEFT,
	CM_RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	Camera(glm::vec3 eyePos, glm::vec3 dir, glm::vec3 up)
	{
		m_eyePos = eyePos;
		m_dir = dir;
		m_worldUp = up;
		m_fov = ZOOM;
		update_camera();
	}

	Camera(glm::vec3 eyePos, glm::vec3 up, float yaw = YAW, float pitch = PITCH)
	{
		m_eyePos = eyePos;
		m_worldUp = up;
		m_yaw = yaw;
		m_pitch = PITCH;
		m_dir = glm::vec3(0.0f, 0.0f, -1.0f);
		m_moveSpeed = SPEED;
		m_mouseSensitivity = SENSITIVITY;
		m_fov = ZOOM;
		update_camera();
	}

	~Camera(){}

	glm::mat4 getViewMat()
	{
		return glm::lookAt(m_eyePos, m_eyePos + m_dir, m_up);
	}

	glm::vec3 getRight()
	{
		return m_right;
	}

	inline void setPos(glm::vec3 eyePos)
	{
		m_eyePos = eyePos;
		update_camera();
	}
	inline glm::vec3 getEyePos() { return m_eyePos; }

	inline void setDir(glm::vec3 dir)
	{
		m_dir = dir;
		update_camera();
	}
	inline glm::vec3 getDir() { return m_dir; }
	inline float getFov() { return m_fov; }

	// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = m_moveSpeed * deltaTime;
		if (direction == CM_FORWARD)
			m_eyePos += m_dir * velocity;
		if (direction == CM_BACKWARD)
			m_eyePos -= m_dir * velocity;
		if (direction == CM_LEFT)
			m_eyePos -= m_right * velocity;
		if (direction == CM_RIGHT)
			m_eyePos += m_right * velocity;
	}

	// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= m_mouseSensitivity;
		yoffset *= m_mouseSensitivity;

		m_yaw += xoffset;
		m_pitch += yoffset;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;
		}

		// update Front, Right and Up Vectors using the updated Euler angles
		update_camera();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset)
	{
		m_fov -= (float)yoffset;
		if (m_fov < 1.0f)
			m_fov = 1.0f;
		if (m_fov > 45.0f)
			m_fov = 45.0f;
	}
private:
	glm::vec3 m_eyePos;
	glm::vec3 m_dir;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	// euler Angles
	float m_yaw;  //Æ«º½½Ç ÈÆYÖáÐý×ª
	float m_pitch;// ¸©Ñö½Ç ÈÆXÖáÐý×ª

	// camera options
	float m_moveSpeed;
	float m_mouseSensitivity;
	float m_fov;

	void update_camera()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		front.y = sin(glm::radians(m_pitch));
		front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
		m_dir = glm::normalize(front);
		m_right = glm::normalize(glm::cross(m_dir, m_worldUp));
		m_up = glm::normalize(glm::cross(m_right, m_dir));
	}
};


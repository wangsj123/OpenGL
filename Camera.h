#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	Camera(glm::vec3 eyePos, glm::vec3 dir, glm::vec3 up);
	~Camera();

	glm::mat4 getViewMat();
	glm::vec3 getRight();

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
private:
	glm::vec3 m_eyePos;
	glm::vec3 m_dir;
	glm::vec3 m_up;
	glm::vec3 m_right;

	void update_camera();
};


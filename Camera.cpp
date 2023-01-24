#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 eyePos, glm::vec3 dir, glm::vec3 up) : m_eyePos(eyePos), m_dir(dir), m_up(up)
{
    update_camera();
}

Camera::~Camera()
{
}

glm::mat4 Camera::getViewMat()
{
    return glm::lookAt(m_eyePos, m_eyePos + m_dir, m_up);
}

glm::vec3 Camera::getRight()
{
    return m_right;
}

void Camera::update_camera()
{
    m_right = glm::normalize(glm::cross(m_dir, m_up));
}

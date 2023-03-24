#include "Light.h"

Light::Light(glm::vec3 color) : m_color(color)
{
}

Light::~Light()
{
}


PointLight::PointLight(glm::vec3 color, glm::vec3 position)
	: Light(color)
	, m_position(position)
{
}

PointLight::~PointLight()
{
}

DirectionLight::DirectionLight(glm::vec3 color, glm::vec3 direction)
	: Light(color)
	, m_direction(direction)
{
}

DirectionLight::~DirectionLight()
{
}

SpotLight::SpotLight(glm::vec3 color, glm::vec3 pos, glm::vec3 direction, float cutOff, float outerCutOff)
	: Light(color)
	, m_position(pos)
	, m_direction(direction)
	, m_cutOff(cutOff)
	, m_outerCutOff(outerCutOff)
{
}

SpotLight::~SpotLight()
{
}
#pragma once
#include "glm/glm.hpp"

class Light
{
public:
	Light(glm::vec3 color);
	virtual ~Light();
public:
	glm::vec3 m_color;
};

class PointLight : public Light
{
public:
	PointLight(glm::vec3 color, glm::vec3 position);
	~PointLight();

public:
	glm::vec3 m_position;
};

class DirectionLight : public Light
{
public:
	DirectionLight(glm::vec3 color, glm::vec3 direction);
	~DirectionLight();

public:
	glm::vec3 m_direction;
};

class SpotLight : public Light
{
public:
	SpotLight(glm::vec3 color, glm::vec3 pos, glm::vec3 direction, float cutOff, float outerCutOff);
	~SpotLight();

public:
	glm::vec3 m_position;
	glm::vec3 m_direction;
	float m_cutOff;
	float m_outerCutOff;
};


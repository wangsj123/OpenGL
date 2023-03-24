#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
uniform mat4 lightSpaceMatrix;

out vec3 oNormal;
out vec2 oTexCoord;
out vec3 oFragPos;
out vec4 oLightSpaceFragPos;
out mat4 oLightSpaceMatrix;

void main()
{
	oNormal = mat3(transpose(inverse(model))) * aNormal;
	oTexCoord = aTexCoord;
	oFragPos = vec3(model * vec4(aPos, 1.0));
	oLightSpaceFragPos = lightSpaceMatrix * vec4(oFragPos,1.0);
	oLightSpaceMatrix = lightSpaceMatrix;

	gl_Position = project * view * model * vec4(aPos, 1.0);
}

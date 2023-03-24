#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 project;
uniform mat4 lightSpaceMatrix;

out VSFS_DATA
{
	vec3 fragPos;
	vec3 normal;
	vec2 texCoord;
	vec4 lightSpaceFragPos;
} vs_out;

void main()
{
	vs_out.normal = mat3(transpose(inverse(model))) * aNormal;
	vs_out.texCoord = aTexCoord;
	vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.lightSpaceFragPos = lightSpaceMatrix * vec4(vs_out.fragPos,1.0);

	gl_Position = project * view * model * vec4(aPos, 1.0);
}

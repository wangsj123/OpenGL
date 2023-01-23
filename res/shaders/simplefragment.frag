#version 330 core

uniform sampler2D texContainer;
uniform sampler2D texAwesomeface;

in vec2 texCoord;
out vec4 FragColor;

void main()
{
	FragColor = mix(vec4(texture(texContainer, texCoord)), vec4(texture(texAwesomeface, texCoord)), 0.3);
}

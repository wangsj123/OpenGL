#include <iostream>
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//const char* vertexShader = "#version 330 core \n"
//"layout(location=0) in vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"gl_Position = vec4(aPos.xyz,1.0f);\n"
//"}\0";
//
//const char* fragmentShader = "#version 330 core\n"
//"out vec4 FragColor;\n"
//"uniform vec4 OurColor;\n"
//"void main()\n"
//"{\n"
//"FragColor = OurColor;\n"
//"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
int main()
{
	std::cout << "get started" << std::endl;
	
	if (glfwInit() == GLFW_FALSE)
	{
		std::cout << "glfw init error" << std::endl;
		return -1;
	}

	GLFWwindow *window = glfwCreateWindow(800, 600, "Learn OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "create glfwwindow error" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	std::cout << glGetString(GL_VERSION) << std::endl;

	//glewInit must after make context
	GLenum ret = glewInit();
	if (ret != GLEW_OK)
	{
		std::cout << "glew init error" << std::endl;
		return -1;
	}

	////shader
	//unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vs, 1, &vertexShader, NULL);
	//glCompileShader(vs);

	//// check for shader compile errors
	//int success;
	//char infoLog[512];
	//glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	//if (!success)
	//{
	//	glGetShaderInfoLog(vs, 512, NULL, infoLog);
	//	std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	//}

	//unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fs, 1, &fragmentShader, NULL);
	//glCompileShader(fs);

	//glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	//if (!success)
	//{
	//	glGetShaderInfoLog(fs, 512, NULL, infoLog);
	//	std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	//}

	//unsigned int shaderProgram = glCreateProgram();
	//glAttachShader(shaderProgram, vs);
	//glAttachShader(shaderProgram, fs);
	//glLinkProgram(shaderProgram);

	//// check for linking errors
	//glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	//if (!success) {
	//	glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	//	std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	//}
	//glDeleteShader(vs);
	//glDeleteShader(fs);

	//unsigned int colorLocation = glGetUniformLocation(shaderProgram, "OurColor");
	
	Shader shader("res/shaders/simplevertex.vs", "res/shaders/simplefragment.frag");

	//
	float verticles[] = {
		-0.5f, -0.5f, 0.0f, 0, 0,
		 0.5f, -0.5f, 0.0f, 2, 0,
		 0.5f, 0.5f, 0.0f, 2, 2,
		-0.5f, 0.5f, 0.0f, 0, 2
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);//bind vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticles), verticles, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);//unbind vbo

	// texture
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // GL_REPEAT GL_CLAMP_TO_EDGE GL_MIRRORED_REPEAT GL_MIRROR_CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	int width, height, nrChannel;
	unsigned char* data = stbi_load("res/textures/container.jpg", &width, &height, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width2, height2, nrChannel2;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data2 = stbi_load("res/textures/awesomeface.png", &width2, &height2, &nrChannel2, 0);
	if (data2)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	shader.use();
	glUniform1i(glGetUniformLocation(shader.getId(), "texContainer"), 0);
	glUniform1i(glGetUniformLocation(shader.getId(), "texAwesomeface"), 1);

	while (!glfwWindowShouldClose(window))
	{
		//process user input
		processInput(window);

		//clear back color
		glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		/*glUseProgram(shaderProgram);
		glUniform4f(colorLocation, 1.0f, 1.0f, .0f, 0.5f);*/
		shader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (window == NULL)
	{
		return;
	}
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
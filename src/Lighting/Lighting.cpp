// Lighting.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../common/Camera.h"
#include "../common/Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Light.h"

const static int SCR_WIDTH = 800;
const static int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
Camera camera(cameraPos, cameraFront, cameraUp);

// timing
double deltaTime = 0.0f;	// time between current frame and last frame
double lastFrame = 0.0f;
float fov = 45.0f;

bool firstMouse = true;
glm::vec2 lastPos = glm::vec2(0.0f, 0.0f);
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;

// lighting
glm::vec3 lightPos = glm::vec3(1.2f, 2.0f, 3.0f);

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

int main()
{
    std::cout << "Lighting Project" << std::endl;
    if (glfwInit() == GLFW_FALSE)
    {
        std::cout << "glfw init error" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OPenGL Lighting", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init error" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices1[] = {
		-0.5f, -0.5f, -0.5f, 0.6f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.6f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.6f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.6f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.6f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.6f, 0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 0.6f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.6f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.6f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.6f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.6f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.6f, 0.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.6f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.6f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.6f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.6f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.6f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 0.6f,

		 0.5f,  0.5f,  0.5f, 0.6f, 0.6f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.6f, 0.6f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.6f, 0.6f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.6f, 0.6f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.6f, 0.6f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.6f, 0.6f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.6f, 0.0f, 0.6f,
		 0.5f, -0.5f, -0.5f, 0.6f, 0.0f, 0.6f,
		 0.5f, -0.5f,  0.5f, 0.6f, 0.0f, 0.6f,
		 0.5f, -0.5f,  0.5f, 0.6f, 0.0f, 0.6f,
		-0.5f, -0.5f,  0.5f, 0.6f, 0.0f, 0.6f,
		-0.5f, -0.5f, -0.5f, 0.6f, 0.0f, 0.6f,

		-0.5f,  0.5f, -0.5f, 0.0f, 0.6f, 0.6f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.6f, 0.6f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.6f, 0.6f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.6f, 0.6f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.6f, 0.6f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.6f, 0.6f
	};

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int diffuseMap;
	glGenTextures(1, &diffuseMap);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannel;
	unsigned char* data = stbi_load("../../res/textures/container2.png", &width, &height, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	unsigned int specMap;
	glGenTextures(1, &specMap);
	glBindTexture(GL_TEXTURE_2D, specMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("../../res/textures/container2_specular.png", &width, &height, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	unsigned int emissionMap;
	glGenTextures(1, &emissionMap);
	glBindTexture(GL_TEXTURE_2D, emissionMap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("../../res/textures/matrix.jpg", &width, &height, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	Shader shader("../../res/shaders/lighting.vs", "../../res/shaders/lighting.frag");
	Shader lightCubeShader("../../res/shaders/light_cube.vs", "../../res/shaders/light_cube.fs");
	shader.use();
	shader.setInt("diffuseMap", 0);
	shader.setInt("specMap", 1);
	shader.setInt("emissionMap", 2);

	glm::mat4 model = glm::mat4(1.0f); // init identity mat
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 project = glm::mat4(1.0f);
	model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 0, 0));
	//glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));

	DirectionLight dirLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(.0f, -1.0f, .0f));
	PointLight pointLight(glm::vec3(1.0f, 1.0f, 0.0f), lightPos);
	SpotLight spotLight(glm::vec3(.0f, .0f, 1.0f), glm::vec3(.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 15.0f, 20.0f);

	while (!glfwWindowShouldClose(window))
	{
		// pre calculate
		double curTime = glfwGetTime();
		deltaTime = curTime - lastFrame;
		lastFrame = curTime;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		/*glUniform4f(glGetUniformLocation(shader.getId(), "lightColor"), 1.0f, 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(shader.getId(), "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shader.getId(), "objColor"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(shader.getId(), "eyePos"), camera.getEyePos().x, camera.getEyePos().y, camera.getEyePos().z);*/
		shader.setVec3("dirLight.color", dirLight.m_color);
		shader.setVec3("dirLight.dir", dirLight.m_direction);
		shader.setVec3("pointLight.color", pointLight.m_color);
		shader.setVec3("pointLight.pos", pointLight.m_position);
		shader.setVec3("spotLight.pos", spotLight.m_position);
		shader.setVec3("spotLight.color", spotLight.m_color);
		shader.setVec3("spotLight.dir", spotLight.m_direction);
		shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotLight.m_outerCutOff)));
		shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(spotLight.m_cutOff)));
		shader.setVec3("eyePos", camera.getEyePos());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionMap);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));

		view = camera.getViewMat();
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));

		project = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(project));

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// also draw the lamp object
		lightCubeShader.use();
		glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(project));
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		glUniformMatrix4fv(glGetUniformLocation(lightCubeShader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(lightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();


    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (window == NULL)
	{
		return;
	}
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (window == NULL)
	{
		return;
	}

	if (firstMouse)
	{
		firstMouse = false;
		lastPos.x = xPos;
		lastPos.y = yPos;
	}

	double deltaX = xPos - lastPos.x;
	double deltaY = yPos - lastPos.y;
	lastPos.x = xPos;
	lastPos.y = yPos;

	float sensitivity = 0.1f; // change this value to your liking
	deltaX *= sensitivity;
	deltaY *= sensitivity;

	yaw += deltaX;
	pitch -= deltaY;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	camera.setDir(cameraFront);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	fov -= (float)yOffset;
	if (fov < 1.0f)
	{
		fov = 1.0f;
	}
	if (fov > 60.0f)
	{
		fov = 60.0f;
	}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	float cameraSpeed = static_cast<float>(2.5 * deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * glm::normalize((glm::cross(cameraFront, cameraUp)));
		camera.setPos(cameraPos);
	}
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * glm::normalize((glm::cross(cameraFront, cameraUp)));
		camera.setPos(cameraPos);
	}
	else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraFront;
		camera.setPos(cameraPos);
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraFront;
		camera.setPos(cameraPos);
	}
}

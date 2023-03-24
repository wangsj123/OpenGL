#pragma once
#include <string>
//#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const char* vertexFile, const char* fragFile)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open file
			vShaderFile.open(vertexFile);
			fShaderFile.open(fragFile);
			std::stringstream vShaderStream, fShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();

			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (const std::ifstream::failure& e)
		{
			std::cout << "ERROR::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// compile shader
		unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vShaderCode, NULL);
		glCompileShader(vs);
		checkCompileErrors(vs, "VERTEX");

		unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fShaderCode, NULL);
		glCompileShader(fs);
		checkCompileErrors(fs, "FRAGMENT");

		// shader Program
		m_id = glCreateProgram();
		glAttachShader(m_id, vs);
		glAttachShader(m_id, fs);
		glLinkProgram(m_id);
		checkCompileErrors(m_id, "PROGRAM");

		// delete the shaders
		glDeleteShader(vs);
		glDeleteShader(fs);
	}
	~Shader() {};

	void use()
	{
		glUseProgram(m_id);
	}
	unsigned int getId() { return m_id; }

public:
	void setBool(const std::string& name, bool value)
	{
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value)
	{
		glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value)
	{
		glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
	}

	void setVec2(const std::string& name, glm::vec2 vec)
	{
		glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &vec[0]);
	}

	void setVec2(const std::string& name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
	}

	void setVec3(const std::string& name, glm::vec3 vec)
	{
		glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &vec[0]);
	}

	void setVec3(const std::string& name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
	}

	void setVec4(const std::string& name, glm::vec4 vec)
	{
		glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &vec[0]);
	}

	void setVec4(const std::string& name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
	}

	void setMatrix2(const std::string& name, glm::mat2 mat)
	{
		glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMatrix3(const std::string& name, glm::mat3 mat)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void setMatrix4(const std::string& name, glm::mat4 mat)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
private:
	unsigned int m_id;
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n-- -------------------------------- --" << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINK_ERROR of type: " << type << "\n" << infoLog << "\n-- ----------------------------- --" << std::endl;
			}
		}
	}
};


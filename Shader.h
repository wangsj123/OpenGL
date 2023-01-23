#pragma once
#include <string>
class Shader
{
public:
	Shader(const char* vertexFile, const char* fragFile);
	~Shader();

	void use();
	unsigned int getId() { return m_id; }
private:
	unsigned int m_id;
	void checkCompileErrors(unsigned int shader, std::string type);
};


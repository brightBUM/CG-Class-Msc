#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
private:
	void checkCompileErrors(unsigned int shader, std::string type);
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	
};


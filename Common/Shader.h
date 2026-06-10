#pragma once
#define Log(x) std::cout<<x<<std::endl;
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
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	void SetFloat(const std::string& name, float value);
	void SetInt(const std::string& name, int value);
	void use();

};


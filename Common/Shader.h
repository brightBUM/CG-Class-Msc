#pragma once
#define Log(x) std::cout<<x<<std::endl;
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include"glm/glm.hpp"
#include"glm/gtc/type_ptr.hpp"

class Shader
{
private:
	void checkCompileErrors(unsigned int shader, std::string type);
public:
	unsigned int ID;
	Shader();
	Shader(const char* vertexPath, const char* fragmentPath);
	void SetFloat(const std::string& name, float value);
	void SetVec3(const std::string& name, glm::vec3 value);
	void SetInt(const std::string& name, int value);
	void SetMat4(const std::string& name, glm::mat4 matrix);
	void use();

};


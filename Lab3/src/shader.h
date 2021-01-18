#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader
{
public:
    unsigned int ID;

    Shader();
    Shader(const char* vertexShaderFile, const char* fragmentShaderFile);

	Shader& Use();

    void SetFloat(std::string name, float value, bool useShader = false);
    void SetInteger(std::string name, int value, bool useShader = false);
    void SetVector2f(std::string name, float x, float y, bool useShader = false);
    void SetVector2f(std::string name, const glm::vec2& value, bool useShader = false);
    void SetVector3f(std::string name, float x, float y, float z, bool useShader = false);
    void SetVector3f(std::string name, const glm::vec3& value, bool useShader = false);
    void SetVector4f(std::string name, float x, float y, float z, float w, bool useShader = false);
    void SetVector4f(std::string name, const glm::vec4& value, bool useShader = false);
    void SetMatrix4(std::string name, const glm::mat4& matrix, bool useShader = false);

    
private:
    void createShader(const char* vertexShaderFile, const char* fragmentShaderFile);
    void compile(const char* vertexShaderCode, const char* fragmentShaderCode);
    void checkCompileErrors(unsigned int object, std::string type);
};
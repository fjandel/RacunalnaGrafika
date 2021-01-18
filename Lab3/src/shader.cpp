#include "shader.h"

#include <iostream>
#include <sstream>
#include <fstream>


Shader::Shader()
{
    createShader("res/shaders/sprite.vs", "res/shaders/sprite.frag");
}

Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    createShader(vertexShaderFile, fragmentShaderFile);
}

Shader& Shader::Use()
{
    glUseProgram(this->ID);
    return *this;
}

void Shader::SetFloat(std::string name, float value, bool useShader)
{
    if (useShader) this->Use();
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::SetInteger(std::string name, int value, bool useShader)
{
    if (useShader)
        this->Use();
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}
void Shader::SetVector2f(std::string name, float x, float y, bool useShader)
{
    if (useShader) this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
}
void Shader::SetVector2f(std::string name, const glm::vec2& value, bool useShader)
{
    if (useShader) this->Use();
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
}
void Shader::SetVector3f(std::string name, float x, float y, float z, bool useShader)
{
    if (useShader) this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
}
void Shader::SetVector3f(std::string name, const glm::vec3& value, bool useShader)
{
    if (useShader) this->Use();
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::SetVector4f(std::string name, float x, float y, float z, float w, bool useShader)
{
    if (useShader) this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
}
void Shader::SetVector4f(std::string name, const glm::vec4& value, bool useShader)
{
    if (useShader) this->Use();
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(std::string name, const glm::mat4& matrix, bool useShader)
{
    if (useShader) this->Use();
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, false, glm::value_ptr(matrix));
}


void Shader::createShader(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    std::string vertexShaderCode;
    std::string fragmentShaderCode;
    try
    {
        std::ifstream vertexShaderFile(vertexShaderFile);
        std::ifstream fragmentShaderFile(fragmentShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexShaderCode = vShaderStream.str();
        fragmentShaderCode = fShaderStream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }

    compile(vertexShaderCode.c_str(), fragmentShaderCode.c_str());
}

void Shader::compile(const char* vertexShaderCode, const char* fragmentShaderCode)
{
    unsigned int vertexShader;
    unsigned int fragmentShader;

    // vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "Vertex");

    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "Fragment");

    // shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);
    glLinkProgram(this->ID);
    checkCompileErrors(this->ID, "Program");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::checkCompileErrors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "Program")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}
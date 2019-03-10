/*================================================================
* Description 自定义着色器类实现
* Email huliuworld@yahoo.com
* Created on Sat Feb 23 2019 1:23:20
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vertexShaderFile.open(vertexPath);
        std::stringstream vertexShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        vertexShaderFile.close();
        vertexCode = vertexShaderStream.str();

        fragmentShaderFile.open(fragmentPath);
        std::stringstream fragmentShaderStream;
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        fragmentShaderFile.close();
        fragmentCode = fragmentShaderStream.str();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    const char *vertexShaderCode = vertexCode.c_str();
    const char *fragmentShaderCode = fragmentCode.c_str();
    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    this->checkCompileErrors(vertexShader, GL_VERTEX_SHADER);
    // fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    this->checkCompileErrors(fragmentShader, GL_FRAGMENT_SHADER);
    // program shader
    this->programShader = glCreateProgram();
    glAttachShader(this->programShader, vertexShader);
    glAttachShader(this->programShader, fragmentShader);
    glLinkProgram(this->programShader);
    this->checkProgramLinkError(programShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {

}

// active the shader
void Shader::useProgram() {
    glUseProgram(this->programShader);
}

// set uniform value
void Shader::setUniformBool(const std::string &name, bool value) const {
    GLint location = glGetUniformLocation(this->programShader, name.c_str());
    if (location < 0) {
        std::cerr << "set uniform " << name << " error" << std::endl;
    }
    glUniform1i(location, (GLint)value);
}

void Shader::setUniformInt(const std::string &name, GLint value) const {
    GLint location = glGetUniformLocation(this->programShader, name.c_str());
    if (location < 0) {
        std::cerr << "set uniform " << name << " error" << std::endl;
    }
    glUniform1i(location, value);
}

void Shader::setUniformFloat(const std::string &name, GLfloat value) const {
    GLint location = glGetUniformLocation(this->programShader, name.c_str());
    if (location < 0) {
        std::cerr << "set uniform " << name << " error" << std::endl;
    }
    glUniform1f(location, value);
}

// check shader compilation errors
void Shader::checkCompileErrors(GLuint shader, GLenum shaderType) {
    GLint compileSuccess = 0;
    char compileFailureInfo[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
    std::string shaderName = "";
    switch (shaderType) {
        case GL_VERTEX_SHADER:
            shaderName = "vertex";
            break;
        case GL_FRAGMENT_SHADER:
            shaderName = "fragment";
            break;
        default:
            std::cerr << "shader type error : " << shaderType << std::endl;
            break;
    }
    if (!compileSuccess) {
        glGetShaderInfoLog(shader, 1024, NULL, compileFailureInfo);
        std::cout << "compile " << shaderName << " shader error : " << compileFailureInfo << std::endl;
    } else {
        std::cout << "compile " << shaderName << " shader success" << std::endl;
    }
}


void Shader::checkProgramLinkError(GLuint shader) {
    GLint linkSuccess = 0;
    char linkFailureInfo[1024];
    glGetProgramiv(shader, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(shader, 1024, NULL, linkFailureInfo);
        std::cout << "link program error : " << linkFailureInfo << std::endl;
    } else {
        std::cout << "link program success" << std::endl;
    }
}

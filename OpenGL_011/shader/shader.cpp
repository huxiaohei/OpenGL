/*================================================================
* Description 自定义着色器类实现
* Email huliuworld@yahoo.com
* Created on Wed Mar 06 2019 23:16:56
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        vertexShaderFile.open(vertexPath);
        std::stringstream vertexStringStream;
        vertexStringStream << vertexShaderFile.rdbuf();
        vertexShaderFile.close();
        vertexCode = vertexStringStream.str();

        fragmentShaderFile.open(fragmentPath);
        std::stringstream fragmentStringStream;
        fragmentStringStream << fragmentShaderFile.rdbuf();
        fragmentShaderFile.close();
        fragmentCode = fragmentStringStream.str();
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    const char *vertexSourceCode = vertexCode.c_str();
    const char *fragmentSourceCode = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSourceCode, NULL);
    glCompileShader(vertexShader);
    this->checkCompile(vertexShader, GL_VERTEX_SHADER);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSourceCode, NULL);
    glCompileShader(fragmentShader);
    this->checkCompile(fragmentShader, GL_FRAGMENT_SHADER);

    this->programShader = glCreateProgram();
    glAttachShader(this->programShader, vertexShader);
    glAttachShader(this->programShader, fragmentShader);
    glLinkProgram(programShader);
    glCompileShader(this->programShader);
    this->checkProgramLink(this->programShader);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {

}

void Shader::useProgram() {
    glUseProgram(this->programShader);
}

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


void Shader::checkCompile(GLuint shader, GLenum shaderType) {
    GLint compileSuccess = 0;
    char compileFailureInfo[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess) {
        glGetShaderInfoLog(shader, 1024, NULL, compileFailureInfo);
        std::cerr << "compile " << "share " << shaderType << " failure : " << compileFailureInfo << std::endl;
    }
}

void Shader::checkProgramLink(GLuint shader) {
    GLint linkSuccess = 0;
    char linkFailureInfo[1024];
    glGetProgramiv(shader, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(shader, 1024, NULL, linkFailureInfo);
        std::cout << "link program error : " << linkFailureInfo << std::endl;
    }
}

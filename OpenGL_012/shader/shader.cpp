/*================================================================
* Description 自定义着色器类实现
* Email huliuworld@yahoo.com
* Created on Wed Mar 06 2019 23:16:56
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::fstream vertexShaderFile;
    std::fstream fragmentShaderFile;
    vertexShaderFile.exceptions(std::fstream::failbit | std::fstream::badbit);
    fragmentShaderFile.exceptions(std::fstream::failbit | std::fstream::badbit);
    try {
        vertexShaderFile.open(vertexPath);
        std::stringstream vertexStream;
        vertexStream << vertexShaderFile.rdbuf();
        vertexShaderFile.close();
        vertexCode = vertexStream.str();

        fragmentShaderFile.open(fragmentPath);
        std::stringstream fragmentStream;
        fragmentStream << fragmentShaderFile.rdbuf();
        fragmentShaderFile.close();
        fragmentCode = fragmentStream.str();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        ;
    }
    const char *vertexShaderSourceCode = vertexCode.c_str();
    const char *fragmentShaderSourceCode = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceCode, NULL);
    glCompileShader(vertexShader);
    this->checkShaderCompileError(vertexShader, GL_VERTEX_SHADER);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
    glCompileShader(fragmentShader);
    this->checkShaderCompileError(fragmentShader, GL_FRAGMENT_SHADER);

    this->programShader = glCreateProgram();
    glAttachShader(this->programShader, vertexShader);
    glAttachShader(this->programShader, fragmentShader);
    glLinkProgram(this->programShader);
    glCompileShader(this->programShader);
    this->checkProgramLink(this->programShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {}

void Shader::checkShaderCompileError(GLuint shader, GLenum type) {
    GLint compileSuccess = 0;
    char compileFailureInfo[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess) {
        glGetShaderInfoLog(shader, 1024, NULL, compileFailureInfo);
        std::cout << "compile " << type << " failure : " << compileFailureInfo
                  << std::endl;
    } else {
        std::cout << "compile " << type << " success" << std::endl;
    }
}

void Shader::checkProgramLink(GLuint shader) {
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

void Shader::useProgram() { glUseProgram(this->programShader); }

void Shader::setUniformVec4(const std::string &name, const ShaderVec4 &vec4) {
    GLuint location = glGetUniformLocation(this->programShader, name.c_str());
    if (location < 0) {
        std::cerr << "set unifrom " << name << " error" << std::endl;
        return;
    }
    glUniform4f(location, vec4.a, vec4.b, vec4.c, vec4.d);
}
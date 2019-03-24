/*================================================================
* Description 自定义着色器类实现
* Email huliuworld@yahoo.com
* Created on Tue Mar 19 2019 23:43:1
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::fstream vertexFile;
    std::fstream fragmentFile;
    vertexFile.exceptions(std::fstream::failbit | std::fstream::badbit);
    fragmentFile.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::string vertexCode;
    std::string fragmentCode;
    try {
        vertexFile.open(vertexPath);
        std::stringstream vertexStringStream;
        vertexStringStream << vertexFile.rdbuf();
        vertexFile.close();
        vertexCode = vertexStringStream.str();

        fragmentFile.open(fragmentPath);
        std::stringstream fragmentStringStream;
        fragmentStringStream << fragmentFile.rdbuf();
        fragmentFile.close();
        fragmentCode = fragmentStringStream.str();
    } catch (const std::exception &e) {
        std::cerr << "read shader source code error : " << e.what() << std::endl;
    }

    const char *vertexShaderSource = vertexCode.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileError(vertexShader, GL_VERTEX_SHADER);

    const char *fragmentShaderSource = fragmentCode.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileError(fragmentShader, GL_FRAGMENT_SHADER);

    programShader = glCreateProgram();
    glAttachShader(programShader, vertexShader);
    glAttachShader(programShader, fragmentShader);
    glLinkProgram(programShader);
    checkProgramLink(programShader);
    glCompileShader(programShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::useProgram() { glUseProgram(programShader); }

GLint Shader::getAttributeLocation(const char *attributeName) {
    return glGetAttribLocation(programShader, attributeName);
}

GLint Shader::getUniformLocation(const char *uniformName) {
    return glGetUniformLocation(programShader, uniformName);
}

void Shader::checkShaderCompileError(GLuint shader, GLenum type) {
    GLint compileSuccess = 0;
    char compileFailureInfo[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess) {
        glGetShaderInfoLog(shader, 1024, NULL, compileFailureInfo);
        std::cerr << "compile failure " << type << " failure "
                  << compileFailureInfo << std::endl;
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
        std::cerr << "link program failure " << linkFailureInfo << std::endl;
    } else {
        std::cout << "link program success" << std::endl;
    }
}

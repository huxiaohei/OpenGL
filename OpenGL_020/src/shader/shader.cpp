/*================================================================
* Description 自定义着色器实现
* Email huliuworld@yahoo.com
* Created on Sun Apr 21 2019 22:19:7
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader.hpp"
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    std::fstream vertexFile;
    std::fstream fragmentFile;
    vertexFile.exceptions(std::fstream::badbit | std::fstream::failbit);
    fragmentFile.exceptions(std::fstream::badbit | std::fstream::failbit);
    std::string vertexStr;
    std::string fragmentStr;
    try {
        vertexFile.open(vertexPath);
        std::stringstream vertexStream;
        vertexStream << vertexFile.rdbuf();
        vertexFile.close();
        vertexStr = vertexStream.str();

        fragmentFile.open(fragmentPath);
        std::stringstream fragmentStream;
        fragmentStream << fragmentFile.rdbuf();
        fragmentFile.close();
        fragmentStr = fragmentStream.str();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    const char *vertexCode = vertexStr.c_str();
    const char *fragmentCode = fragmentStr.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileError(vertexShader, GL_VERTEX_SHADER);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileError(fragmentShader, GL_FRAGMENT_SHADER);

    _programId = glCreateProgram();
    glAttachShader(_programId, vertexShader);
    glAttachShader(_programId, fragmentShader);
    glLinkProgram(_programId);
    glCompileShader(_programId);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
}

void Shader::use() {
    glUseProgram(_programId);
}

GLint Shader::getUniformLocation(const std::string &name) const {
    GLuint location = glGetUniformLocation(_programId, name.c_str());
    return location;
}

void Shader::updateVertexAttributePointer(const char *attributeName, GLint size,
                                          GLenum type, GLboolean normalized,
                                          GLsizei stride,
                                          const GLvoid *pointer) {
    GLint location = glGetAttribLocation(_programId, attributeName);
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, type, normalized, stride, pointer);
}

void Shader::updateUniformIntByName(const std::string &name, int value) const {
    GLint location = getUniformLocation(name);
    glUniform1i(location, value);
}

void Shader::updateUniformMatrix4fvByName(const std::string &name, int count, bool transpose, const float *value) const {
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, count, transpose, value);
}

bool Shader::checkShaderCompileError(GLuint shader, GLenum type) {
    GLint success = 0;
    char compileFailureInfo[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, compileFailureInfo);
        std::cerr << "compile shader " << shader << " type: " << type << " error: " << compileFailureInfo << std::endl;
        return false;
    }
    return true;
}

bool Shader::checkProgramLinkError() {
    GLint success = 0;
    char linkFailureInfo[1024];
    glGetProgramiv(_programId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(_programId, 1024, NULL, linkFailureInfo);
        std::cerr << "link program " << _programId << " error: " << linkFailureInfo << std::endl;
        return false;
    }
    return true;
}
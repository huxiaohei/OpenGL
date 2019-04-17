/*================================================================
* Description 自定义着色器类实现
* Email huliuworld@yahoo.com
* Created on Sun Apr 14 2019 22:24:44
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader.h"
#include <fstream>
#include <sstream>
#include <string>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
    std::fstream vertexFile;
    std::fstream fragmentFile;
    vertexFile.exceptions(std::fstream::failbit | std::fstream::badbit);
    fragmentFile.exceptions(std::fstream::failbit | std::fstream::badbit);
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
        std::cerr << e.what() << '\n';
        return;
    }
    const char *vertexCode = vertexStr.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileError(vertexShader, GL_VERTEX_SHADER);

    const char *fragmentCode = fragmentStr.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileError(fragmentShader, GL_FRAGMENT_SHADER);

    _programId = glCreateProgram();
    glAttachShader(_programId, vertexShader);
    glAttachShader(_programId, fragmentShader);
    glLinkProgram(_programId);
    checkProgramLinkError(_programId);
    glCompileShader(_programId);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::useProgram() { glUseProgram(_programId); }

bool Shader::checkShaderCompileError(GLuint shader, GLenum shaderType) {
    GLint compileSuccess = 0;
    char compileFailureInfo[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileSuccess);
    if (!compileSuccess) {
        glGetShaderInfoLog(shader, 1024, NULL, compileFailureInfo);
        std::cerr << "compile shader: " << shader << " type: " << shaderType
                  << " error" << std::endl;
        std::cerr << "compile failure info: " << compileFailureInfo << std::endl;
        return false;
    }
    return true;
}

bool Shader::checkProgramLinkError(GLuint programId) {
    GLint linkSuccess = 0;
    char linkFailureInfo[1024];
    glGetProgramiv(programId, GL_LINK_STATUS, &linkSuccess);
    if (!linkSuccess) {
        glGetProgramInfoLog(programId, 1024, NULL, linkFailureInfo);
        std::cerr << "link program: " << programId << " error" << std::endl;
        std::cerr << "link failure info: " << linkFailureInfo << std::endl;
        return false;
    }
    return true;
}

GLint Shader::getAttributeLocation(const char *attributeName) {
    return glGetAttribLocation(_programId, attributeName);
}

GLint Shader::getUniformLocation(const char *name) {
    GLuint location = glGetUniformLocation(_programId, name);
    return location;
}

void Shader::updateUniformMatrix4fvByName(const char *name, int count, bool transpose, const float *value) {
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, count, transpose, value);
}

void Shader::updateVertexAttributePointer(const char *attributeName, GLint size,
                                          GLenum type, GLboolean normalized,
                                          GLsizei stride,
                                          const GLvoid *pointer) {
    GLint loc = glGetAttribLocation(_programId, attributeName);
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, size, type, normalized, stride, pointer);
}

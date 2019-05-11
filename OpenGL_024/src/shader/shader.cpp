/*================================================================
* Description 自定义着色器实现
* Email huliuworld@yahoo.com
* Created on Sun Apr 21 2019 22:19:7
* Copyright (c) 2019 刘虎
================================================================*/

#include "shader.hpp"
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile) {
    std::fstream vertexFileHandle;
    std::fstream fragmentFileHandle;
    vertexFileHandle.exceptions(std::fstream::failbit | std::fstream::badbit);
    fragmentFileHandle.exceptions(std::fstream::failbit | std::fstream::badbit);
    std::string vertexShaderStr;
    std::string fragmentShaderStr;
    try {
        vertexFileHandle.open(vertexShaderFile);
        std::stringstream vertexStrStream;
        vertexStrStream << vertexFileHandle.rdbuf();
        vertexFileHandle.close();
        vertexShaderStr = vertexStrStream.str();

        fragmentFileHandle.open(fragmentShaderFile);
        std::stringstream fragmentStrStream;
        fragmentStrStream << fragmentFileHandle.rdbuf();
        fragmentFileHandle.close();
        fragmentShaderStr = fragmentStrStream.str();
    } catch (std::exception &e) {
        std::cerr << "read file error: " << e.what() << std::endl;
    }

    const char *vertexShaderCode = vertexShaderStr.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileError(vertexShader);

    const char *fragmentShaderCode = fragmentShaderStr.c_str();
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileError(fragmentShader);

    _programId = glCreateProgram();
    glAttachShader(_programId, vertexShader);
    glAttachShader(_programId, fragmentShader);
    glLinkProgram(_programId);
    checkProgramLinkError();
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

void Shader::setVertexAttributePointer(const char *attributeName, GLint size,
                                       GLenum type, GLboolean normalized,
                                       GLsizei stride,
                                       const GLvoid *pointer) {
    GLint location = glGetAttribLocation(_programId, attributeName);
    if (location < 0) {
        std::cout << "attrib location error" << std::endl;
        return;
    }
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, type, normalized, stride, pointer);
}

bool Shader::setUniformIntByName(const std::string &name, int value) const {
    GLint location = getUniformLocation(name);
    if (location < 0) {
        std::cerr << "set uniform: " << name << " error" << std::endl;
        return false;
    }
    glUniform1i(location, value);
    return true;
}

bool Shader::setIniformFloatByName(const std::string &name, float value) const {
    GLint location = getUniformLocation(name);
    if (location < 0) {
        std::cerr << "set uniform: " << name << " error" << std::endl;
        return false;
    }
    glUniform1f(location, value);
    return true;
}

bool Shader::setUniformFloatVec3ByName(const std::string &name, glm::vec3 &value) const {
    GLint location = getUniformLocation(name);
    if (location < 0) {
        std::cerr << "set uniform: " << name << " error" << std::endl;
        return false;
    }
    glUniform3fv(location, 1, &value[0]);
    return true;
}

bool Shader::setUniformFloat3ByName(const std::string &name, float x, float y, float z) const {
    GLint location = getUniformLocation(name);
    if (location < 0) {
        std::cerr << "set uniform: " << name << " error" << std::endl;
        return false;
    }
    glUniform3f(location, x, y, z);
    return true;
}

bool Shader::setUniformFloatVec4ByName(const std::string &name, glm::vec4 &value) const {
    GLint location = getUniformLocation(name);
    if (location < 0) {
        std::cerr << "set uniform: " << name << " error" << std::endl;
        return false;
    }
    glUniform4fv(location, 1, &value[0]);
    return true;
}

bool Shader::setUniformFloat4ByName(const std::string &name, float x, float y, float z, float w) const {
    GLint location = getUniformLocation(name);
    if (location < 0) {
        std::cerr << "set uniform: " << name << " error" << std::endl;
        return false;
    }
    glUniform4f(location, x, y, z, w);
    return true;
}

void Shader::setUniformMatrix4fvByName(const std::string &name, int count, bool transpose, const float *value) const {
    GLint location = getUniformLocation(name);
    glUniformMatrix4fv(location, count, transpose, value);
}

bool Shader::checkShaderCompileError(GLuint shader) const {
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char compileFailureInfo[1024];
        glGetShaderInfoLog(shader, 1024, NULL, compileFailureInfo);
        std::cerr << "compile shader " << shader << " error: " << compileFailureInfo << std::endl;
        return false;
    }
    return true;
}

bool Shader::checkProgramLinkError() const {
    GLint success = 0;
    glGetProgramiv(_programId, GL_LINK_STATUS, &success);
    if (!success) {
        char linkFailureInfo[1024];
        glGetShaderInfoLog(_programId, 1024, NULL, linkFailureInfo);
        std::cerr << "link program " << _programId << " error: " << linkFailureInfo << std::endl;
        return false;
    }
    return true;
}
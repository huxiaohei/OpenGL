/*================================================================
* Description 自定义着色器头文件
* Email huliuworld@yahoo.com
* Created on Sun Apr 21 2019 22:16:20
* Copyright (c) 2019 刘虎
================================================================*/

#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>

class Shader {

private:
protected:
    GLuint _programId;

public:
    Shader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);
    ~Shader();
    void use();
    bool checkShaderCompileError(GLuint shader) const;
    bool checkProgramLinkError() const;
    GLint getUniformLocation(const std::string &name) const;
    void setVertexAttributePointer(const char *attributeName, GLint size,
                                   GLenum type, GLboolean normalized,
                                   GLsizei stride, const GLvoid *pointer);
    bool setUniformIntByName(const std::string &name, int value) const;
    bool setUniformFloatVec3ByName(const std::string &name, glm::vec3 &value) const;
    bool setUniformFloat3ByName(const std::string &name, float x, float y, float z) const;
    bool setUniformFloatVec4ByName(const std::string &name, glm::vec4 &value) const;
    bool setUniformFloat4ByName(const std::string &name, float x, float y, float z, float w) const;
    void setUniformMatrix4fvByName(const std::string &name, int count, bool transpose, const float *value) const;
};

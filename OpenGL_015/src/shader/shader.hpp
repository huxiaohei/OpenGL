/*================================================================
* Description 自定义着色器类头文件
* Email huliuworld@yahoo.com
* Created on Fri Apr 19 2019 0:16:43
* Copyright (c) 2019 刘虎
================================================================*/

#include <glad/glad.h>
#include <iostream>

class Shader {

private:
protected:
    GLuint _programId;

public:
    Shader(char const *vertexPath, char const *fragmentPath);
    ~Shader();
    void use();
    bool checkShaderCompileError(GLuint shader, GLenum type);
    bool checkProgramLinkError(GLuint program);
    GLint getUniformLocation(const std::string &name) const;
    void updateVertexAttributePointer(const char *attributeName, GLint size,
                                      GLenum type, GLboolean normalized,
                                      GLsizei stride, const GLvoid *pointer);
    void updateUniformIntByName(const std::string &name, int value) const;
    void updateUniformMatrix4fvByName(const std::string &name, int count, bool transpose, const float *value) const;
};
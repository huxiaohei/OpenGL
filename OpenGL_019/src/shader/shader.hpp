/*================================================================
* Description 自定义着色器头文件
* Email huliuworld@yahoo.com
* Created on Sun Apr 21 2019 22:16:20
* Copyright (c) 2019 刘虎
================================================================*/

#include <glad/glad.h>
#include <iostream>

class Shader {

private:
protected:
    GLuint _programId;

public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader();
    void use();
    bool checkShaderCompileError(GLuint shader, GLenum type);
    bool checkProgramLinkError();
    GLint getUniformLocation(const std::string &name) const;
    void updateVertexAttributePointer(const char *attributeName, GLint size,
                                      GLenum type, GLboolean normalized,
                                      GLsizei stride, const GLvoid *pointer);
    void updateUniformIntByName(const std::string &name, int value) const;
    void updateUniformMatrix4fvByName(const std::string &name, int count, bool transpose, const float *value) const;
};

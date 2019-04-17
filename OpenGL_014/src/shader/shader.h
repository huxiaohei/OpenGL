/*================================================================
* Description 自定义着色器类头文件
* Email huliuworld@yahoo.com
* Created on Sun Apr 14 2019 22:23:58
* Copyright (c) 2019 刘虎
================================================================*/

#include <glad/glad.h>
#include <iostream>

class Shader {

private:
protected:
    GLuint _programId;

public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
    bool checkShaderCompileError(GLuint shader, GLenum shaderType);
    bool checkProgramLinkError(GLuint programId);
    void useProgram();
    GLint getAttributeLocation(const char *attributeName);
    GLint getUniformLocation(const char *name);
    void updateUniformMatrix4fvByName(const char *name, int count, bool transpose, const float *value);
    void updateVertexAttributePointer(const char *attributeName, GLint size,
                                      GLenum type, GLboolean normalized,
                                      GLsizei stride, const GLvoid *pointer);
};

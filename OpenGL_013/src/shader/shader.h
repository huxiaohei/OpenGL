/*================================================================
* Description 自定义着色器类头文件
* Email huliuworld@yahoo.com
* Created on Tue Mar 19 2019 23:42:5
* Copyright (c) 2019 刘虎
================================================================*/

#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>

class Shader {
  private:
    GLuint programShader;

  public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
    void useProgram();
    GLint getAttributeLocation(const char *attributeName);
    GLint getUniformLocation(const char *uniformName);
    void checkShaderCompileError(GLuint shader, GLenum type);
    void checkProgramLink(GLuint shader);
};

#endif

/*================================================================
* Description 自定义着色器类头文件
* Email huliuworld@yahoo.com
* Created on Wed Mar 13 2019 0:29:51
* Copyright (c) 2019 刘虎
================================================================*/

#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>
#include <iostream>

typedef struct {
    GLfloat a;
    GLfloat b;
    GLfloat c;
    GLfloat d;
} ShaderVec4;

class Shader {
  public:
    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader();
    void checkShaderCompileError(GLuint shader, GLenum type);
    void checkProgramLink(GLuint shader);
    void useProgram();
    void setUniformVec4(const std::string &name, const ShaderVec4 &vec4);

  private:
    GLuint programShader;
};

#endif
/*================================================================
* Description 自定义着色器类头文件
* Email huliuworld@yahoo.com
* Created on Wed Mar 06 2019 23:13:39
* Copyright (c) 2019 刘虎
================================================================*/

#ifndef _SHADER_H_
#define _SHADER_H_

#include <iostream>
#include <glad/glad.h>

class Shader {
    public:
        Shader(const char *vertexPath, const char *fragmentPath);
        ~Shader();
        void checkCompile(GLuint shader, GLenum shaderType);
        void checkProgramLink(GLuint shader);
        void useProgram();
        void setUniformBool(const std::string &name, bool value) const;
        void setUniformInt(const std::string &name, GLint value) const;
        void setUniformFloat(const std::string &name, GLfloat value) const;

      protected:
        GLuint programShader;
};

#endif

/*================================================================
* Description 点光源顶点着色器
* Email huliuworld@yahoo.com
* Created on Fri Jun 07 2019 17:55:8
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 pos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
    gl_Position = projection * view * model *vec4(pos, 1.0f);
}


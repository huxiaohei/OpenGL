/*================================================================
* Description 顶点着色器源码
* Email huliuworld@yahoo.com
* Created on Fri Mar 01 2019 23:48:19
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out vec4 aColor;

void main () {

    gl_Position = vec4(pos, 1.0f);
    aColor = vec4(color, 1.0f);
    // aColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
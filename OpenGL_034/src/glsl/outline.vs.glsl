/*================================================================
* Description 轮廓顶点着色器
* Email huliuworld@yahoo.com
* Created on Thu Jun 27 2019 23:17:22
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
    gl_Position = projection * view * model * vec4(pos, 1.0);
}

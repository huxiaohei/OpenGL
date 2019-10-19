/*================================================================
* Description 面剔除
* Email huliuworld@yahoo.com
* Created on Fri Oct 04 2019 11:48:20
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texturePos;

out vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    texcoord = texturePos;
}

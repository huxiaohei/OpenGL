/*================================================================
* Description 离屏渲染
* Email huliuworld@yahoo.com
* Created on Sat Oct 19 2019 17:41:13
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texturePos;

out vec2 texcoord;

void main () {
 
    gl_Position = vec4(pos, 1.0);
    texcoord = texturePos;
}

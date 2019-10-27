/*================================================================
* Description 箱子片段着色器
* Email huliuworld@yahoo.com
* Created on Sun Oct 27 2019 14:43:1
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

in vec2 texcoord;
uniform sampler2D texture2d;
out vec4 fragmentColor;

void main() {
    fragmentColor = texture(texture2d, texcoord);
}



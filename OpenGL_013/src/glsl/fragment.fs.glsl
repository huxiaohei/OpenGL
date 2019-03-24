/*================================================================
* Description 片段着色器源码
* Email huliuworld@yahoo.com
* Created on Sat Mar 23 2019 1:41:21
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

precision mediump float;

uniform sampler2D image;

in vec2 vTexcoord;
out vec4 fragmentColor;

void main () {
    fragmentColor = texture(image, vTexcoord);
}

/*================================================================
* Description 片段着色器源码
* Email huliuworld@yahoo.com
* Created on Wed Apr 17 2019 1:5:37
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

precision mediump float;

uniform sampler2D texture0;
uniform sampler2D texture1;

in vec2 vTexcoord;
out vec4 fragmentColor;

void main () {
    fragmentColor = mix(texture(texture0, vTexcoord), texture(texture1, vTexcoord), 0.2);
}

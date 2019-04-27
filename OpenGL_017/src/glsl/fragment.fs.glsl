/*================================================================
* Description 片段着色器
* Email huliuworld@yahoo.com
* Created on Wed Apr 24 2019 0:36:25
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

precision mediump float;

in vec2 vTexcoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

out vec4 fragmentColor;

void main () {
    fragmentColor = mix(texture(texture0, vTexcoord), texture(texture1, vTexcoord), 0.2);
}

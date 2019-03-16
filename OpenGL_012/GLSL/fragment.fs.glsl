/*================================================================
* Description 片段着色器源码
* Email huliuworld@yahoo.com
* Created on Wed Mar 13 2019 23:50:15
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

uniform vec4 color;
out vec4 fragmentColor;

void main () {
    fragmentColor = color;
}



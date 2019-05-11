/*================================================================
* Description 光源片段着色器
* Email huliuworld@yahoo.com
* Created on Sat May 04 2019 22:35:21
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

uniform vec4 lightColor;
out vec4 fragmentColor;

void main () {
    fragmentColor = lightColor;
}

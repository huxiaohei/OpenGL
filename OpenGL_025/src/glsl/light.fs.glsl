/*================================================================
* Description 光源片段着色器
* Email huliuworld@yahoo.com
* Created on Sun May 19 2019 21:48:39
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

uniform vec3 lightColor;
out vec4 fragmentColor;

void main () {
    fragmentColor = vec4(lightColor, 1.0f);
}

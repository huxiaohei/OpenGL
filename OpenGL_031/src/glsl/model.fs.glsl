/*================================================================
* Description 网格片段着色器
* Email huliuworld@yahoo.com
* Created on Thu Jun 27 2019 23:17:39
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

in vec2 texcoord;
out vec4 fragmentColor;
uniform sampler2D texture_diffuse1;

void main () {
    fragmentColor = texture(texture_diffuse1, texcoord);
}


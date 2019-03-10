/*================================================================
* Description 片段着色器
* Email huliuworld@yahoo.com
* Created on Fri Mar 01 2019 23:49:24
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

in vec4 aColor;

out vec4 fragmentColor;

void main () {
    
    fragmentColor = aColor;
}

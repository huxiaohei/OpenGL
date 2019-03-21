/*================================================================
* Description 顶点着色器源码
* Email huliuworld@yahoo.com
* Created on Fri Mar 22 2019 0:23:38
* Copyright (c) 2019 刘虎
================================================================*/

#version 410 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in mat4 model_matrix;




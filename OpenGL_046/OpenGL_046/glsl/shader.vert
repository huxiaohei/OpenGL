/*================================================================
* Description 
* Email huxiaoheigame@yahoo.com
* Created on 2019 11 5 23:25:57
* Copyright (c) 2019 huxiaohei
================================================================*/

#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

out Material {
	vec2 texcoord;
	vec3 normal;
	vec3 position;
} outVert;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectionMat;

void main () {
    gl_Position = projectionMat * viewMat * modelMat * vec4(position, 1.0);
    outVert.texcoord = texcoord;
	outVert.normal = normal;
	outVert.position = position;
}


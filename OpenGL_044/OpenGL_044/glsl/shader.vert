/*================================================================
* Description ¶¥µã×ÅÉ«Æ÷
* Email huxiaoheigame@yahoo.com
* Created on 2019 11 2 22:28:47
* Copyright (c) 2019 huxiaohei
================================================================*/

#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectMat;

out Material {
	vec2 texcoords;
	vec3 normal;
} materialVert;


void main () {
	gl_Position = projectMat * viewMat * modelMat * vec4(position, 1.0f);
	materialVert.texcoords = texcoords;
	mat3 normalMatrix = mat3(transpose(inverse(viewMat * modelMat)));
    materialVert.normal = vec3(projectMat * vec4(normalMatrix * normal, 0.0f));
}



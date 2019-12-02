/*================================================================
* Description 
* Email huxiaoheigame@yahoo.com
* Created on 2019 11 1 17:14:28
* Copyright (c) 2019 huxiaohei
================================================================*/

#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projectMat;

out Material {
	vec3 position;
	vec2 texcoords;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
} outMaterialVert;


void main() {
	gl_Position = projectMat * viewMat * modelMat * vec4(position, 1.0);
	outMaterialVert.normal = normal;
	outMaterialVert.texcoords = texcoords;
}


/*================================================================
* Description ¶¥µã×ÅÉ«Æ÷
* Email huxiaoheigame@yahoo.com
* Created on 2019 10 27 0:46:43
* Copyright (c) 2019 huxiaohei
================================================================*/

#version 410 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

out VertexShaderOut {
	vec3 color;
} vertexShaderOut;

void main() {
	vertexShaderOut.color = color;
	gl_Position = vec4(pos, 1.0);
}


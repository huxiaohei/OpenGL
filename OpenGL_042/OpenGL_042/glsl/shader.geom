/*================================================================
* Description ������ɫ��
* Email huxiaoheigame@yahoo.com
* Created on 2019 10 27 0:56:33
* Copyright (c) 2019 huxiaohei
================================================================*/

#version 410 core

layout (points) in;
in VertexShaderOut {
	vec3 color;
} gs_in[];
layout (triangle_strip, max_vertices = 5) out;
out vec3 color;


void buildHouse(vec4 position){
	color = gs_in[0].color;
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:����
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:����
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:����
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:����
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:����
    EmitVertex();
    EndPrimitive();
}

void main() {
	buildHouse(gl_in[0].gl_Position);
}



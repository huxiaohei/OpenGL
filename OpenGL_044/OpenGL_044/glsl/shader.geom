/*================================================================
* Description 几何着色器
* Email huxiaoheigame@yahoo.com
* Created on 2019 11 2 23:42:51
* Copyright (c) 2019 huxiaohei
================================================================*/

#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in Material {
	vec2 texcoords;
	vec3 normal;
} inGeom[];

const float MAGNITUDE = 0.0025;

void generateLine(int index) {
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(inGeom[index].normal, 0.0) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main() {
    generateLine(0);
    generateLine(1);
    generateLine(2);
}





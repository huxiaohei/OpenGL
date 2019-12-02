/*================================================================
* Description ¼ÆËã×ÅÉ«Æ÷
* Email huxiaoheigame@yahoo.com
* Created on 2019 10 27 0:56:33
* Copyright (c) 2019 huxiaohei
================================================================*/

#version 410 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in Material {
	vec3 position;
	vec2 texcoords;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
} inMaterialGeom[];

uniform float time;

out vec2 texcoords;

vec3 getNormal(){
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

vec4 explode(vec3 normal, int index) {
	float magnitude = 2.0;
	vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
	return gl_in[index].gl_Position + vec4(direction, 0.0);
}

void main() {
	vec3 normal = getNormal();
	for (int i = 0; i < 3; i++) {
		gl_Position = explode(normal, i);
		texcoords = inMaterialGeom[i].texcoords;
		EmitVertex();
	}
	EndPrimitive();
}



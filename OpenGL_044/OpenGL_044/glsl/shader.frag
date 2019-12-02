/*================================================================
* Description Æ¬¶Ï×ÅÉ«Æ÷
* Email huxiaoheigame@yahoo.com
* Created on 2019 11 2 23:12:59
* Copyright (c) 2019 huxiaohei
================================================================*/

#version 410 core

in Material {
	vec2 texcoords;
	vec3 normal;
} inFrag;

uniform sampler2D texture_diffuse1;
uniform int drawNormal;
out vec4 fragColor;

void main () {
	if (drawNormal == 1) {
		fragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	} else {
		fragColor = texture(texture_diffuse1, inFrag.texcoords) * 1.2;
	}
}



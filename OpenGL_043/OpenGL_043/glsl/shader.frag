/*================================================================
* Description Æ¬¶Î×ÅÉ«Æ÷
* Email huxiaoheigame@yahoo.com
* Created on 2019 10 27 0:52:45
* Copyright (c) 2019 huxiaohei
================================================================*/

#version 410 core

in vec2 texcoords;

uniform sampler2D texture_diffuse1;
out vec4 fragColor;

void main() {
	fragColor = texture(texture_diffuse1, texcoords) * 1.2;
}


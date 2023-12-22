#version 330 core 

out vec4 color;

in vec2 texCoord;

uniform sampler2D tex0;

uniform float textureAdd_x;

void main()
{
	color = texture(tex0, vec2(texCoord.x + textureAdd_x, texCoord.y));
}


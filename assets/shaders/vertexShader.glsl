#version 430 core

layout (location = 0) in vec2 objectPos;

layout (location = 1) in vec2 aTex;


out vec2 texCoord;


void main() 
{
	gl_Position = vec4(objectPos, 0.0, 1.0);
	texCoord  = aTex;
}


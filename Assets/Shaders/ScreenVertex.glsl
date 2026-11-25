#version 420 core
#include "Attributes.glslh"

uniform sampler2D diffuseTex;

out Vertex{
	vec2 texCoord;
} OUT;

void main(void) {
	gl_Position = vec4(position.xyz, 1.0);
	OUT.texCoord = texCoord;
}
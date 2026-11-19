#version 420 core
#include "TestStructs.glslh"
#include "LightStructs.h"

const int max_lights = 16;

uniform sampler2D diffuseColourTex;
uniform bool hasTexture;
uniform vec4 albedoColour;

layout (std140, binding = 0) uniform Constants {
    FrameConstants frameConstants;
};

layout (std140, binding = 1) uniform Lights {
	LightData lights[max_lights];
};

in Vertex {
	vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
} IN;

out vec4 fragColour;

#include "phong.glslh"

void main(void) {
	vec4 diffuseColour;
	if(hasTexture){
		diffuseColour = texture(diffuseColourTex, IN.texCoord);
	}
	else{
		diffuseColour = albedoColour;
	}
	phong(diffuseColour, fragColour);
}
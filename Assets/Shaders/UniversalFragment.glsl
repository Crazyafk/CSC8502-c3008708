#version 420 core
#include "TestStructs.glslh"

uniform sampler2D diffuseColourTex;
uniform bool hasTexture;
uniform vec4 albedoColour;

uniform vec3 pointLightPos;
uniform float pointLightRadius;
uniform vec4 pointLightColour;

layout (std140, binding = 0) uniform Constants {
    FrameConstants frameConstants;
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
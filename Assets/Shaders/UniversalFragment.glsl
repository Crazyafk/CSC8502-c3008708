#version 330 core
uniform sampler2D diffuseTex;
uniform bool hasTexture;
uniform vec4 albedoColour;

in Vertex {
	vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void) {
	if(hasTexture){
		fragColour = texture(diffuseTex, IN.texCoord);
	}
	else{
		fragColour = albedoColour;
	}
}
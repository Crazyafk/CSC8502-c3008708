#version 330 core
uniform sampler2D diffuseTex;
uniform bool hasTexture;
uniform vec4 albedoColour;

in Vertex {
	vec2 texCoord;
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
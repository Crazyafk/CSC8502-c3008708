#version 420
#include "Attributes.glslh"
#include "TestStructs.glslh"

uniform mat4 modelMatrix;

layout (std140, binding = 0) uniform Constants {
    FrameConstants frameConstants;
};

out Vertex {
    vec2 texCoord;
} OUT;

void main(void) {
    mat4 mvp = frameConstants.viewProjMatrix * modelMatrix;
    gl_Position = mvp * vec4(position.xyz, 1.0);
    OUT.texCoord = texCoord;
}

#version 420
#include "Attributes.glslh"
#include "TestStructs.glslh"

uniform mat4 modelMatrix;

layout (std140, binding = 2) uniform Constants {
    FrameConstants frameConstants;
};

void main(void) {
    mat4 mvp = frameConstants.projMatrix * frameConstants.viewMatrix * modelMatrix;
    gl_Position = mvp * vec4(position, 1.0);
}
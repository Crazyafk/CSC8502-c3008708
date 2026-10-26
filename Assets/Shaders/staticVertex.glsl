#version 420
#include "TestStructs.h"

uniform mat4 modelMatrix;

layout (std140, binding = 0) uniform Constants {
    FrameConstants frameConstants;
};

in vec3 position;
in vec2 texCoord;
in vec4 tangent;
in vec3 normal;

out Vertex {
    vec2 texCoord;
} OUT;

void main(void) {
    mat4 mvp = frameConstants.projMatrix * frameConstants.viewMatrix * modelMatrix;
    gl_Position = mvp * vec4(position.xyz, 1.0);
    OUT.texCoord = texCoord;
}

#version 420
#include "Attributes.h"
#include "TestStructs.h"

uniform mat4 modelMatrix;

layout (std140, binding = 0) uniform Constants {
    FrameConstants frameConstants;
};

uniform mat4 joints[128];

out Vertex {
    vec2 texCoord;
} OUT;

void main(void) {
    vec4 localPos   = vec4(position, 1.0f);
    vec4 skelPos    = vec4(0, 0, 0, 0);

    for (int i = 0; i < 4; ++i) {
        int     jointIndex  = jointIndices[i];
        float   jointWeight = jointWeights[i];

        skelPos += joints[jointIndex] * localPos * jointWeight;
    }
    mat4 mvp = frameConstants.projMatrix * frameConstants.viewMatrix * modelMatrix;
    gl_Position = mvp * vec4(skelPos.xyz, 1.0);
    OUT.texCoord = texCoord;
}

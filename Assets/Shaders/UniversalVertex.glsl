#version 420
#include "Attributes.glslh"
#include "TestStructs.glslh"

uniform mat4 modelMatrix;
uniform bool isAnimated;

layout (std140, binding = 0) uniform Constants {
    FrameConstants frameConstants;
};

uniform mat4 joints[128];

out Vertex {
    vec2 texCoord;
} OUT;

void main(void) {
    mat4 mvp = frameConstants.viewProjMatrix * modelMatrix;
    vec4 localPos   = vec4(position, 1.0f);
    vec4 pos = vec4(0, 0, 0, 0);
    if(isAnimated) {
        for (int i = 0; i < 4; ++i) {
            int     jointIndex  = jointIndices[i];
            float   jointWeight = jointWeights[i];

            pos += joints[jointIndex] * localPos * jointWeight;
        }
    }
    else{
        pos = localPos;
    }
    gl_Position = mvp * vec4(pos.xyz, 1.0);
    OUT.texCoord = texCoord;
}

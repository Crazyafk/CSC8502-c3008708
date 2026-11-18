#version 420
#include "Attributes.glslh"
#include "TestStructs.glslh"

layout (std140, binding = 0) uniform Constants {
    FrameConstants frameConstants;
};

uniform mat4 viewNoTranslation;

out Vertex{
    vec3 ViewDir;
} OUT;

void main()
{
    vec3 pos = position;
    mat4 invProj = inverse(frameConstants.projMatrix);
    pos.xy *= vec2(invProj[0][0], invProj[1][1]);
    pos.z = -1.0f;
    OUT.ViewDir = transpose(mat3(frameConstants.viewMatrix)) * normalize(pos);
    gl_Position = vec4(position, 1.0f);
}
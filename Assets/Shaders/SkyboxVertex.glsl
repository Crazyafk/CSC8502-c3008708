#version 420
#include "Attributes.glslh"
#include "TestStructs.glslh"

layout (std140, binding = 0) uniform Constants {
    FrameConstants frameConstants;
};

uniform mat4 viewNoTranslation;

out vec3 TexCoords;

void main()
{
    TexCoords = position;
    gl_Position = frameConstants.projMatrix * viewNoTranslation * vec4(position, 1.0);
}
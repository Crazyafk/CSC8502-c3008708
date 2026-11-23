#version 420
#include "Attributes.glslh"
#include "TestStructs.glslh"

uniform mat4 modelMatrix;
uniform bool isAnimated;

uniform int debug_nodeID;
uniform float debug_distanceFromCamera;

layout (std140, binding = 0) uniform Constants {
    FrameConstants frameConstants;
};

uniform mat4 joints[128];
uniform vec3 shadowingOriginPos;

out Vertex {
    vec4 colour;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
    vec4 shadowProj;
} OUT;

#include "AnimateSkeletal.glsl"

void main(void) {
    vec4 localPos   = vec4(position, 1.0f);
    vec4 pos = vec4(0, 0, 0, 0);

    //if we don't pretend to use the debug variables they'll be optimised out
    if(debug_nodeID == -1){ localPos = vec4(0,0,0,0); }
    if(debug_distanceFromCamera == -10000){ localPos = vec4(0,0,0,0); }

    skel_Animate(localPos, pos);
    
    vec4 worldPos = modelMatrix * pos;
    gl_Position = frameConstants.viewProjMatrix * worldPos;

    OUT.colour = colour;
    OUT.texCoord = texCoord;
    OUT.worldPos = worldPos.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    OUT.normal = normalize(normalMatrix * normalize(normal));

    //anti-acne shadow biasing
    vec3 viewDir = normalize(shadowingOriginPos - worldPos.xyz);
    vec4 pushVal = vec4(OUT.normal, 0) * dot(viewDir, OUT.normal);
    OUT.shadowProj = frameConstants.shadowMatrix * (worldPos + pushVal);
}

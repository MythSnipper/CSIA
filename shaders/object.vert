#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vWorldPos;
out vec3 vNormal;
out vec2 vUV;

void main(){
    vec4 worldPos = model * vec4(aPos, 1.0);

    vWorldPos = worldPos.xyz;
    //inverse-transpose keeps normals perpendicular to the surface under non-uniform scale (your cube4)
    vNormal = mat3(transpose(inverse(model))) * aNormal;
    vUV = aUV;

    gl_Position = projection * view * worldPos;
}
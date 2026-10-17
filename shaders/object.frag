#version 330 core

out vec4 FragColor;

//don't do anything
uniform mat4 inverseView;
uniform mat4 inverseProjection;
uniform mat4 viewProjection;
uniform vec2 viewportSize;
uniform float spacing;
uniform vec3 cameraPosition;
//


void main(){
    FragColor = vec4(0.6, 0.1, 1.0, 1.0);
}


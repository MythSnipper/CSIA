#version 330 core

out vec4 FragColor;

uniform mat4 inverseView;
uniform mat4 inverseProjection;
uniform mat4 viewProjection;
uniform vec2 viewportSize;
uniform float spacing;
uniform vec3 cameraPosition;

void main(){
    //reconstruct the camera ray.
    vec2 ndc = (gl_FragCoord.xy / viewportSize) * 2.0 - 1.0;

    vec4 rayClip = vec4(ndc, -1.0, 1.0);
    vec4 rayView = inverseProjection * rayClip;
    rayView = vec4(rayView.xy, -1.0, 0.0);

    vec3 rayDirection = normalize(
        (inverseView * rayView).xyz
    );

    // Intersect the ray with the ground plane (y = 0).
    if(abs(rayDirection.y) < 0.0001){
        discard;
    }

    float t = -cameraPosition.y / rayDirection.y;

    if(t <= 0.0){
        discard;
    }

    vec3 worldPosition = cameraPosition + rayDirection * t;

    // Calculate the grid lines in world space.
    vec2 coord = worldPosition.xz / spacing;
    vec2 derivative = fwidth(coord);

    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = 1.0 - min(min(grid.x, grid.y), 1.0);

    // Fade grid lines with distance.
    float fade = 1.0 - smoothstep(20.0, 80.0, t);
    float alpha = line * fade;

    if(alpha < 0.01){
        discard;
    }

    // Calculate depth so scene geometry can occlude the grid.
    vec4 clipPosition = viewProjection * vec4(worldPosition, 1.0);
    float depth = clipPosition.z / clipPosition.w;
    gl_FragDepth = depth * 0.5 + 0.5;

    FragColor = vec4(0.4, 0.4, 0.4, alpha);
}
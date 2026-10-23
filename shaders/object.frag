#version 330 core

in vec3 vWorldPos;
in vec3 vNormal;
in vec2 vUV;

out vec4 FragColor;

//material
uniform vec3 materialColor;
uniform vec2 tiling;
uniform float shininess;
uniform float specularStrength;
uniform sampler2D albedoTexture;

//scene
uniform vec3 cameraPosition;
uniform vec3 lightDirection;   //direction the light travels
uniform vec3 lightColor;
uniform float ambientStrength;

void main(){
    vec3 albedo = texture(albedoTexture, vUV * tiling).rgb * materialColor;

    vec3 normal = normalize(vNormal);
    vec3 toLight = normalize(-lightDirection);
    vec3 toCamera = normalize(cameraPosition - vWorldPos);

    float diffuse = max(dot(normal, toLight), 0.0);

    vec3 halfway = normalize(toLight + toCamera);
    float specular = 0.0;
    if(diffuse > 0.0){
        specular = pow(max(dot(normal, halfway), 0.0), shininess) * specularStrength;
    }

    vec3 lit = albedo * (ambientStrength + diffuse) * lightColor + specular * lightColor;
    FragColor = vec4(lit, 1.0);
}
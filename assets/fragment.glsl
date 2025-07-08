#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec4 WorldPosition;

out vec4 FragColor;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;

uniform sampler2D mapKa;
uniform sampler2D mapKd;
uniform sampler2D mapKs;

uniform float shininess;
uniform float shininess_strength;

void main()
{    
    // Light direction (in view space, since WorldPosition is in view space)
    vec3 lightDir = normalize(vec3(0,-1,0));
    
    // View direction (in view space this is just the negative of the position)
    vec3 viewDir = normalize(-WorldPosition.xyz);
    
    // Halfway vector between light and view directions
    vec3 halfwayDir = normalize(lightDir + viewDir);
    
    // Diffuse calculation
    float diff = max(dot(-lightDir, Normal), 0.0);
    
    // Specular calculation
    float spec = pow(max(dot(halfwayDir, Normal), 0.0), shininess);
    
    vec4 ambient = texture(mapKa, TexCoords) * vec4(Ka, 1);
    vec4 diffuse = texture(mapKd, TexCoords) * vec4(Kd, 1) * diff;
    vec4 specular = texture(mapKs, TexCoords) * vec4(Ks, 1) * spec * shininess_strength;

    FragColor = ambient + diffuse + specular;
}

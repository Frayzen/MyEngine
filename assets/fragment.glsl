#version 330 core

in vec2 TexCoords;
in vec3 Normal;

out vec4 FragColor;

void main()
{    
    vec3 lightdir = vec3(0,-1,0);
    float diff = max(dot(-lightdir, Normal), 0);
    FragColor = vec4(vec3(diff), 1);
}

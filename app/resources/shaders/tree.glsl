//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 5) in mat4 instanceModel;


out vec2 TexCoords;
out vec3 FragPos;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
    TexCoords = aTexCoords;
    gl_Position = projection * view * instanceModel * vec4(aPos, 1.0f);
}

//#shader fragment
#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main() {
    FragColor = vec4(vec3(texture(texture_diffuse1, TexCoords)), 1.0f);
}
//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}

//#shader fragment
#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform float time;

void main() {
    vec3 fireColor = vec3(texture(texture_diffuse1, TexCoords));
    float flicker = 0.8 + 0.2 * (sin(time * 6.0) * sin(time * 1.7) * sin(time * 2.1));
    vec3 emissive = fireColor * flicker;

    FragColor = vec4(emissive, 1.0f);
}
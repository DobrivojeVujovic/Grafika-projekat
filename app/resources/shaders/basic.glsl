//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
//layout (location = 3) in vec3 lightPos;


out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
    TexCoords = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
};

uniform DirLight sun;

void main() {
    vec3 lightDir = normalize(sun.direction);
    vec3 norm = normalize(Normal);

    //ambient component
    vec3 ambient = sun.ambient * vec3(texture(texture_diffuse1, TexCoords));

    //diffuse component
    float diff = max(dot(lightDir, -norm), 0.0);
    vec3 diffuse = sun.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));

    // no specular component

    FragColor = vec4(ambient + diffuse, 1.0);
    //    FragColor = vec4(TexCoords, 0.0, 1.0);
}
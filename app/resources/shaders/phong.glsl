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
uniform sampler2D texture_specular1;
uniform float shininess;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 cameraPos;
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


    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = sun.specular * spec * texture(texture_specular1, TexCoords).rrr;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
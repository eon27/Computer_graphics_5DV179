#version 330

in vec3 Normal;
in vec3 FragPos;

out vec4  fColor;

vec3 lightPos;
vec4 diffuse;
vec4 lightColor;

void
main()
{
    lightColor = vec4(0.4, 0.4, 0.2, 0);
    lightPos = vec3(2.0, 2.5, 5.0);
    float ambientStrength = 0.1;
    vec4 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    vec4 color = ambient + diffuse;

    fColor = color;
}

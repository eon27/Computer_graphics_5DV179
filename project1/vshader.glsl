#version 330

in vec3 vPosition;
in vec3 vNormal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;


out vec3 Normal;
out vec3 FragPos; 

vec4 material;
vec4 camera;

void
main()
{
    material = vec4(1.0, 1.0, 1.0, 0);
    camera = vec4(0.0, 0.0, 2.0, 0.0);
    
    gl_Position = P*V*M*vec4(vPosition, 1.0);
    Normal = vNormal;
    FragPos = vec3(M * vec4(vPosition, 1.0));
}

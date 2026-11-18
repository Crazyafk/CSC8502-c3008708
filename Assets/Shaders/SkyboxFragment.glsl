#version 330 core
uniform samplerCube skybox;

out vec4 FragColor;

in Vertex {
    vec3 ViewDir ;
} IN ;

void main()
{    
    FragColor = texture(skybox, normalize(IN.ViewDir));
}
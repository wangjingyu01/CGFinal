#version 330 core
out vec4 FragColor;

in vec2 UV;
in vec3 posWS;
in vec3 Normal;

uniform bool isselected;//判断是否被选中

void main()
{    
    vec3 final=vec3(0);
    if(isselected)
        final=vec3 (0,1,0);

    FragColor = vec4(final,1);
}
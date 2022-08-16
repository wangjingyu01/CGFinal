#version 330 core
out vec4 FragColor;

in vec2 UV;
in vec3 posWS;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform bool isselected;//判断是否被选中
uniform vec3 cameraPos;

void main()
{    
    vec3 viewDir = normalize(cameraPos - posWS);
    vec3 final=texture(texture_diffuse1, UV).rgb;
    float fresnel=0;
    if(isselected)
    {
        final+=vec3(0,0.2,0);
    }
    FragColor = vec4(final,1);
}
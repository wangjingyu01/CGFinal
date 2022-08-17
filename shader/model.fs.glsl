#version 330 core
out vec4 FragColor;

in vec2 UV;
in vec3 posWS;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform bool isselected;//�ж��Ƿ�ѡ��
uniform vec3 cameraPos;

void main()
{    
    vec3 viewDir = normalize(cameraPos - posWS);
    vec3 final=texture(texture_diffuse1, UV).rgb;
    float fresnel=0;
    if(isselected)
    {
fresnel =1-dot(viewDir,Normal);//��������ʽ����Ե����Ч��
            fresnel=pow(fresnel,3);//Ч��ǿ��
            final+=vec3 (fresnel)*vec3(0,1,0);
    }
    FragColor = vec4(final,1);
}
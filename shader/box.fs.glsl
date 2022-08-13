#version 330 core
out vec4 FragColor;

in vec3 Normal;//����
in vec3 posWS;
in vec2 UV;

struct Material {
    vec3 emissive;//�Է���
    vec3 diffuse;//������ɫ
    vec3 specular;   //�߹�
    float specularIntancity;//�߹�ǿ��
    float shininess;
}; 


struct DirectionalLight//ƽ�й�
{
    vec3 direction;
    vec3 color;
};

struct Spotlight//�۹�� 
{
    vec3 position;//λ��
    vec3 direction;//����
    float outRange;//˥����Χ
    vec3 color;//��ɫ
};

struct PointLight//���Դ
{
    vec3 position;
    float outRange;
    vec3 color;
    float lightIntancity;//����ǿ��
};

uniform vec3 cameraPos;
uniform Material m;

uniform DirectionalLight dirLight;
uniform PointLight pointLight[5];
uniform Spotlight spotlight;

uniform sampler2D texture_diffuse1;//��ͨ��ͼ
uniform sampler2D texture_specular1;//�߹���ͼ
uniform sampler2D texture_normal1;//������ͼ
uniform sampler2D texture_height1;//�߶���ͼ

uniform int LightNum;//���Դ����

void main()
{    
    //���Դ
    vec3 PLight =vec3(0);
    for(int i=0;i<LightNum;i++)
    {
        vec3 LightDir = normalize(pointLight[i].position-posWS);
        float d=length(pointLight[i].position-posWS);//�ƹ������˥��
        d =smoothstep(15,0,d);
        float pointNoL = dot(LightDir, Normal)*d*pointLight[i].lightIntancity;//�ƹ�ǿ��
        PLight += vec3(pointNoL)*pointLight[i].color;
    }

    //ƽ�й�
    float dirNoL =dot(-dirLight.direction,Normal);
    vec3 DLight=vec3(dirNoL)*dirLight.color;

    //�۹��
    vec3 spotDir = normalize(spotlight.position - posWS) ;float theta = dot(spotDir,-spotlight.direction);
    float d=length(spotlight.position-posWS);//�ƹ������˥��
        d =smoothstep(30,0,d);
    float spotNoL = dot(spotDir, Normal)*d;
    float inRange = cos(radians(0.0f));
    float outRange = cos(radians (30.0f)) ;
    vec3 SLight = vec3(smoothstep(outRange,inRange, theta)*spotNoL)*spotlight.color;

    vec3 final =vec3(PLight+SLight+DLight)+vec3(0.1,0.2,0.25);
    FragColor =vec4(DLight,1);
}



    /*vec3 mainTexture=texture(texture_diffuse1,UV).rgb;
    vec3 specularTexture=texture(texture_specular1,UV).rgb;
    vec3 LightDir = normalize(l.position-posWS);
    float NoL = dot(LightDir, Normal);
    NoL=max(0,NoL);
//NoL=NoL*0.5+0.5;//�������ع���ģ�ͣ���Ӱ����û��ô����
//NoL=step(0.5, NoL);//��ͨ��Ⱦ��step������ΪС��0.5����Ϊ0������0.5Ϊ1����ʹ��Ӱ��Ӳ

    // Specular
    vec3 rDir = reflect(-LightDir, Normal);//���䷽��
    vec3 vDir = normalize(cameraPos-posWS);//�ӽǷ���
    float specular = max(0,dot(vDir, rDir));
    specular=pow(specular,m.shininess);

    /*float fresnel =1-dot(vDir,Normal);//��������ʽ����Ե����Ч��
    fresnel=pow(fresnel,2);//Ч��ǿ��
    ʹ��ʱ��final������� +vec3 (fresnel)*vec3(��ɫ)
   
    vec3 final = (vec3(NoL)*mainTexture*m.diffuse*l.color*l.lightIntancity*(1-specularTexture)+m.emissive+l.ambient)
+vec3(specular*m.specularIntancity)*m.specular*l.specular*specularTexture*l.lightIntancity;
FragColor = vec4(final,1);*/

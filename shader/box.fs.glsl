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

struct Light {
    vec3 position;//�Ƶ�λ��
    vec3 ambient;//�ƹ��Է���
    vec3 color;//�ƹ���ɫ
    vec3 specular;//�߹�
    float lightIntancity;//����ǿ��
};

uniform Material m;
uniform Light l;
uniform vec3 cameraPos;


uniform sampler2D texture_diffuse1;//��ͨ��ͼ
uniform sampler2D texture_specular1;//�߹���ͼ
uniform sampler2D texture_normal1;//������ͼ
uniform sampler2D texture_height1;//�߶���ͼ

void main()
{    
    vec3 mainTexture=texture(texture_diffuse1,UV).rgb;
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
    */
    vec3 final = (vec3(NoL)*mainTexture*m.diffuse*l.color*l.lightIntancity*(1-specularTexture)+m.emissive+l.ambient)
+vec3(specular*m.specularIntancity)*m.specular*l.specular*specularTexture*l.lightIntancity;
FragColor = vec4(final,1);
}
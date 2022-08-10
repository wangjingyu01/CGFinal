#version 330 core

//in vec3 vertexColor;//��ȡVS��vertexColor
//in vec3 vColor;
in vec3 Normal;//����
in vec2 UV;
in vec3 posWS;

out vec4 FragColor;// ���Ϊvec4

struct Material {
    sampler2D MainTexture;//����ͼƬ
    sampler2D SpecularTexture;//�߹���ͼ
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

uniform vec3 cameraPos;

uniform Material m;
uniform Light l;


/*uniform float newColor;
uniform sampler2D t1;
uniform sampler2D t2;
uniform float sinTime;*/

void main()
{
vec3 mainTexture=texture(m.MainTexture,UV).rgb;
vec3 specularTexture=texture(m.SpecularTexture,UV).rgb;
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
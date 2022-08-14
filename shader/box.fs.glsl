#version 330 core
out vec4 FragColor;

in vec3 Normal;//����
in vec3 posWS;
in vec2 UV;

struct Material {//������������
	vec3 emissive;//�Է���
	vec3 diffuse;//������ɫ
	vec3 specular;//�߹�
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
	vec3 specular;
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

uniform bool isPointLight;
uniform bool isDirectionalLight;
uniform bool isSpotlight;

uniform sampler2D texture_diffuse;//��ͨ��ͼ
uniform sampler2D texture_specular;//�߹���ͼ
uniform sampler2D texture_normal;//������ͼ
uniform sampler2D texture_height;//�߶���ͼ

uniform int LightNum;//���Դ����


void main()
{
	//���Դ
	vec3 PLight = vec3(0);
	vec3 result =vec3 (0);
	if (isPointLight)//���ж�Ҳ�У��Ż����ܵĲ�����ʡ�Ĳ������ԴҲ��һ�Ѷ���
	{
		for (int i = 0; i < LightNum; i++)
		{
//			vec3 LightDir = normalize(pointLight[i].position - posWS);
//			float d = length(pointLight[i].position - posWS);//�ƹ������˥��
//			d = smoothstep(15, 0, d);
//			float pointNoL = dot(LightDir, Normal) * d * pointLight[i].lightIntancity;//�ƹ�ǿ��
//			PLight += vec3(pointNoL) * pointLight[i].color;
	// ambient
    vec3 ambient = 0.2 * texture(texture_diffuse, UV).rgb;
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pointLight[i].position);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight[i].color * diff *pointLight[i].lightIntancity* texture(texture_diffuse, UV).rgb;  
    // specular
    vec3 viewDir = normalize(cameraPos - posWS);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), m.shininess);
    vec3 specular = pointLight[i].specular * spec * texture(texture_specular, UV).rgb;  
        
    result += (ambient + diffuse + specular);
		}
	}


	//ƽ�й�
	vec3 DLight = vec3(0);
	if (isDirectionalLight)
	{
		float dirNoL = dot(-dirLight.direction, Normal);
		DLight = vec3(dirNoL) * dirLight.color;
	}


	//�۹��
	vec3 SLight = vec3(0);
	if (isSpotlight)
	{
		vec3 spotDir = normalize(spotlight.position - posWS);
		float theta = dot(spotDir, -spotlight.direction);
		float d = length(spotlight.position - posWS);//�ƹ������˥��
		d = smoothstep(30, 0, d);
		float spotNoL = dot(spotDir, Normal) * d;
		float inRange = cos(radians(0.0f));
		float outRange = cos(radians(30.0f));
		vec3 SLight = vec3(smoothstep(outRange, inRange, theta) * spotNoL) * spotlight.color;
	}
	vec3 MultiLightFinal = vec3(PLight + SLight + DLight);


	vec3 MaterialFinal = vec3(0);
	if (isPointLight)
	{
		for (int i = 0; i < LightNum; i++)
		{
			vec3 mainTexture = texture(texture_diffuse, UV).rgb;
			vec3 specularTexture = texture(texture_specular, UV).rgb;
			vec3 LightDir = normalize(pointLight[i].position - posWS);
			float NoL = dot(LightDir, Normal);
			NoL = max(0, NoL);
			//NoL=NoL*0.5+0.5;//�������ع���ģ�ͣ���Ӱ����û��ô����
			//NoL=step(0.5, NoL);//��ͨ��Ⱦ��step������ΪС��0.5����Ϊ0������0.5Ϊ1����ʹ��Ӱ��Ӳ

			// Specular
			vec3 rDir = reflect(-LightDir, Normal);//���䷽��
			vec3 vDir = normalize(cameraPos - posWS);//�ӽǷ���
			float specular = max(0, dot(vDir, rDir));
			specular = pow(specular, m.shininess);

			/*float fresnel =1-dot(vDir,Normal);//��������ʽ����Ե����Ч��
		fresnel=pow(fresnel,2);//Ч��ǿ��
		ʹ��ʱ��final������� +vec3 (fresnel)*vec3(��ɫ)*/

			vec3 PointLightFinal = (vec3(NoL) * mainTexture * m.diffuse * pointLight[i].color
				* pointLight[i].lightIntancity * (1 - specularTexture) + m.emissive)
				+ vec3(specular * m.specularIntancity) * m.specular * pointLight[i].specular * specularTexture * pointLight[i].lightIntancity;//���Դ


			MaterialFinal += vec3(PointLightFinal);
		}
	}
		FragColor = vec4(result, 1);

}
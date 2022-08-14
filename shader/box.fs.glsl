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
    vec3 color;//��ɫ
    float lightIntancity;//����ǿ��
};

struct PointLight//���Դ
{
    vec3 specular;
    vec3 position;
    float outRange;
    vec3 color;
    float lightIntancity;
};

uniform vec3 cameraPos;
uniform Material m;

uniform DirectionalLight dirLight;
uniform PointLight pointLight[5];
uniform Spotlight spotLight;

uniform bool isDirectionalLight;
uniform bool isPointLight;
uniform bool isSpotlight;

uniform sampler2D texture_diffuse;//��ͨ��ͼ
uniform sampler2D texture_specular;//�߹���ͼ
uniform sampler2D texture_normal;//������ͼ
uniform sampler2D texture_height;//�߶���ͼ

uniform int LightNum;//���Դ����


void main()
{
    vec3 result = vec3(0);
    //ƽ�й�
    vec3 DLight = vec3(0);
    if (isDirectionalLight)//���ж�Ҳ�У��Ż����ܵĲ�����ʡ�Ĳ���Ҳ��һ�Ѷ���
    {
        vec3 ambient = 0.2 * texture(texture_diffuse, UV).rgb;
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(-dirLight.direction);  
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = dirLight.color * diff *2* texture(texture_diffuse, UV).rgb;  
        // specular
        vec3 viewDir = normalize(cameraPos - posWS);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), m.shininess);
        vec3 specular = dirLight.color * spec * texture(texture_specular, UV).rgb;  
        
        result += ambient + diffuse + specular;
    }


    //���Դ
    vec3 PLight = vec3(0);
    if (isPointLight)
    {
        for (int i = 0; i < LightNum; i++)
        {
            // ambient
            vec3 ambient = 0.2 * texture(texture_diffuse, UV).rgb;

            // diffuse 
            vec3 norm = normalize(Normal);
            vec3 lightDir = normalize(pointLight[i].position - posWS);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = pointLight[i].color * 2 * diff * texture(texture_diffuse, UV).rgb;

            // specular
            vec3 viewDir = normalize(cameraPos - posWS);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), m.shininess);
            vec3 specular = pointLight[i].specular * spec * texture(texture_specular, UV).rgb;

            //�ƹ������˥����������ø��߼���Ķ�����ĺ���hhh��
            float distance = length(pointLight[i].position - posWS);
            float attenuation = 1.0 / (1 + 0.09f * distance + 0.032f * (distance * distance));
            result +=(ambient + diffuse + specular)*vec3(attenuation)*vec3(pointLight[i].lightIntancity);
        }
    }



    //�۹��
   vec3 SLight = vec3(0);
    if (isSpotlight)
    {
        // ambient
        vec3 ambient = 0.2 * texture(texture_diffuse, UV).rgb;

        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(cameraPos - posWS);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = 2 * diff * texture(texture_diffuse, UV).rgb;

        // specular
        vec3 viewDir = normalize(cameraPos - posWS);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), m.shininess);
        vec3 specular = spotLight.color * spec * texture(texture_specular, UV).rgb;

        // spotlight (soft edges)
        float theta = dot(lightDir, normalize(-spotLight.direction));
        float epsilon = (cos(radians(12.5f)) - cos(radians(17.5f)));
        float intensity = clamp((theta - cos(radians(17.5f))) / epsilon, 0.0, 1.0);
        diffuse *= intensity;
        specular *= intensity;

        // ˥����������ø�������İɣ�
         float d = length(cameraPos - posWS);
    	 d = smoothstep(35, 0, d) * 3 *spotLight.lightIntancity;
         result += (ambient + diffuse + specular)*vec3(d);
    }

FragColor = vec4(result, 1);

}
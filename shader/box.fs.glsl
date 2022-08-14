#version 330 core
out vec4 FragColor;

in vec3 Normal;//法线
in vec3 posWS;
in vec2 UV;

struct Material {//物体自身性质
    vec3 emissive;//自发光
    vec3 diffuse;//物体颜色
    vec3 specular;//高光
    float specularIntancity;//高光强度
    float shininess;
};


struct DirectionalLight//平行光
{
    vec3 direction;
    vec3 color;
};

struct Spotlight//聚光灯 
{
    vec3 position;//位置
    vec3 direction;//方向
    vec3 color;//颜色
    float lightIntancity;//光照强度
};

struct PointLight//点光源
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

uniform sampler2D texture_diffuse;//普通贴图
uniform sampler2D texture_specular;//高光贴图
uniform sampler2D texture_normal;//法线贴图
uniform sampler2D texture_height;//高度贴图

uniform int LightNum;//点光源数量


void main()
{
    vec3 result = vec3(0);
    //平行光
    vec3 DLight = vec3(0);
    if (isDirectionalLight)//不判断也行，优化性能的操作，省的不开也算一堆东西
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


    //点光源
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

            //灯光随距离衰减（这里就用个高级点的二次项的好了hhh）
            float distance = length(pointLight[i].position - posWS);
            float attenuation = 1.0 / (1 + 0.09f * distance + 0.032f * (distance * distance));
            result +=(ambient + diffuse + specular)*vec3(attenuation)*vec3(pointLight[i].lightIntancity);
        }
    }



    //聚光灯
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

        // 衰减（这里就用个二次项的吧）
         float d = length(cameraPos - posWS);
    	 d = smoothstep(35, 0, d) * 3 *spotLight.lightIntancity;
         result += (ambient + diffuse + specular)*vec3(d);
    }

FragColor = vec4(result, 1);

}
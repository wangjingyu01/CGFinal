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
	float outRange;//衰减范围
	vec3 color;//颜色
};

struct PointLight//点光源
{
	vec3 specular;
	vec3 position;
	float outRange;
	vec3 color;
	float lightIntancity;//光照强度
};

uniform vec3 cameraPos;
uniform Material m;

uniform DirectionalLight dirLight;
uniform PointLight pointLight[5];
uniform Spotlight spotlight;

uniform bool isPointLight;
uniform bool isDirectionalLight;
uniform bool isSpotlight;

uniform sampler2D texture_diffuse;//普通贴图
uniform sampler2D texture_specular;//高光贴图
uniform sampler2D texture_normal;//法线贴图
uniform sampler2D texture_height;//高度贴图

uniform int LightNum;//点光源数量


void main()
{
	//点光源
	vec3 PLight = vec3(0);
	vec3 result =vec3 (0);
	if (isPointLight)//不判断也行，优化性能的操作，省的不开点光源也算一堆东西
	{
		for (int i = 0; i < LightNum; i++)
		{
//			vec3 LightDir = normalize(pointLight[i].position - posWS);
//			float d = length(pointLight[i].position - posWS);//灯光随距离衰减
//			d = smoothstep(15, 0, d);
//			float pointNoL = dot(LightDir, Normal) * d * pointLight[i].lightIntancity;//灯光强度
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


	//平行光
	vec3 DLight = vec3(0);
	if (isDirectionalLight)
	{
		float dirNoL = dot(-dirLight.direction, Normal);
		DLight = vec3(dirNoL) * dirLight.color;
	}


	//聚光灯
	vec3 SLight = vec3(0);
	if (isSpotlight)
	{
		vec3 spotDir = normalize(spotlight.position - posWS);
		float theta = dot(spotDir, -spotlight.direction);
		float d = length(spotlight.position - posWS);//灯光随距离衰减
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
			//NoL=NoL*0.5+0.5;//半兰伯特光照模型（阴影部分没那么暗）
			//NoL=step(0.5, NoL);//卡通渲染（step函数意为小于0.5部分为0，大于0.5为1），使光影更硬

			// Specular
			vec3 rDir = reflect(-LightDir, Normal);//反射方向
			vec3 vDir = normalize(cameraPos - posWS);//视角方向
			float specular = max(0, dot(vDir, rDir));
			specular = pow(specular, m.shininess);

			/*float fresnel =1-dot(vDir,Normal);//菲涅尔公式，边缘发光效果
		fresnel=pow(fresnel,2);//效果强度
		使用时在final后面加上 +vec3 (fresnel)*vec3(颜色)*/

			vec3 PointLightFinal = (vec3(NoL) * mainTexture * m.diffuse * pointLight[i].color
				* pointLight[i].lightIntancity * (1 - specularTexture) + m.emissive)
				+ vec3(specular * m.specularIntancity) * m.specular * pointLight[i].specular * specularTexture * pointLight[i].lightIntancity;//点光源


			MaterialFinal += vec3(PointLightFinal);
		}
	}
		FragColor = vec4(result, 1);

}
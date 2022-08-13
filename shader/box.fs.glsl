#version 330 core
out vec4 FragColor;

in vec3 Normal;//法线
in vec3 posWS;
in vec2 UV;

struct Material {
    vec3 emissive;//自发光
    vec3 diffuse;//物体颜色
    vec3 specular;   //高光
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

uniform sampler2D texture_diffuse1;//普通贴图
uniform sampler2D texture_specular1;//高光贴图
uniform sampler2D texture_normal1;//法线贴图
uniform sampler2D texture_height1;//高度贴图

uniform int LightNum;//点光源数量

void main()
{    
    //点光源
    vec3 PLight =vec3(0);
    for(int i=0;i<LightNum;i++)
    {
        vec3 LightDir = normalize(pointLight[i].position-posWS);
        float d=length(pointLight[i].position-posWS);//灯光随距离衰减
        d =smoothstep(15,0,d);
        float pointNoL = dot(LightDir, Normal)*d*pointLight[i].lightIntancity;//灯光强度
        PLight += vec3(pointNoL)*pointLight[i].color;
    }

    //平行光
    float dirNoL =dot(-dirLight.direction,Normal);
    vec3 DLight=vec3(dirNoL)*dirLight.color;

    //聚光灯
    vec3 spotDir = normalize(spotlight.position - posWS) ;float theta = dot(spotDir,-spotlight.direction);
    float d=length(spotlight.position-posWS);//灯光随距离衰减
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
//NoL=NoL*0.5+0.5;//半兰伯特光照模型（阴影部分没那么暗）
//NoL=step(0.5, NoL);//卡通渲染（step函数意为小于0.5部分为0，大于0.5为1），使光影更硬

    // Specular
    vec3 rDir = reflect(-LightDir, Normal);//反射方向
    vec3 vDir = normalize(cameraPos-posWS);//视角方向
    float specular = max(0,dot(vDir, rDir));
    specular=pow(specular,m.shininess);

    /*float fresnel =1-dot(vDir,Normal);//菲涅尔公式，边缘发光效果
    fresnel=pow(fresnel,2);//效果强度
    使用时在final后面加上 +vec3 (fresnel)*vec3(颜色)
   
    vec3 final = (vec3(NoL)*mainTexture*m.diffuse*l.color*l.lightIntancity*(1-specularTexture)+m.emissive+l.ambient)
+vec3(specular*m.specularIntancity)*m.specular*l.specular*specularTexture*l.lightIntancity;
FragColor = vec4(final,1);*/

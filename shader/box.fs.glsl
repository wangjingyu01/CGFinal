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

struct Light {
    vec3 position;//灯的位置
    vec3 ambient;//灯光自发光
    vec3 color;//灯光颜色
    vec3 specular;//高光
    float lightIntancity;//光照强度
};

uniform Material m;
uniform Light l;
uniform vec3 cameraPos;


uniform sampler2D texture_diffuse1;//普通贴图
uniform sampler2D texture_specular1;//高光贴图
uniform sampler2D texture_normal1;//法线贴图
uniform sampler2D texture_height1;//高度贴图

void main()
{    
    vec3 mainTexture=texture(texture_diffuse1,UV).rgb;
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
    */
    vec3 final = (vec3(NoL)*mainTexture*m.diffuse*l.color*l.lightIntancity*(1-specularTexture)+m.emissive+l.ambient)
+vec3(specular*m.specularIntancity)*m.specular*l.specular*specularTexture*l.lightIntancity;
FragColor = vec4(final,1);
}
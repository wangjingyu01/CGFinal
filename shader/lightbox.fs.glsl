#version 330 core

out vec4 FragColor;// 输出为vec4
uniform vec3 lightColor;
uniform float lightIntancity;//光照强度

void main()
{
	FragColor = vec4(lightColor*lightIntancity,1);
}
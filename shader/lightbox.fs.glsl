#version 330 core

out vec4 FragColor;// ���Ϊvec4

uniform vec3 lightColor;
uniform float lightIntancity;//����ǿ��
uniform int LightNum;

void main()
{
	FragColor = vec4(lightColor*lightIntancity/3,1);
}
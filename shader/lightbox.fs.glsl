#version 330 core

out vec4 FragColor;// ���Ϊvec4
uniform vec3 lightColor;
uniform float lightIntancity;//����ǿ��

void main()
{
	FragColor = vec4(lightColor*lightIntancity,1);
}
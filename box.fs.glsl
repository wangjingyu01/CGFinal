#version 330 core

in vec2 UV;
in vec3 Normal;
out vec4 FragColor;// Êä³öÎªvec4

uniform sampler2D t0;
void main()
{
	vec3 t0 = texture2D(t0, UV).xyz;
	FragColor = vec4(t0,1);
}
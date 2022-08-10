#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUv;

out vec2 UV;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    gl_Position =  P*V*M*vec4(aPos, 1.0);
    UV=aUv;
}
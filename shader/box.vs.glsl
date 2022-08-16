#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec2 UV;
out vec3 Normal;
out vec3 posWS;//world position ->WS

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 cameraPos;

void main()
{
    gl_Position = P*V*M * vec4(aPos, 1.0);
    posWS=(M*vec4(aPos,1.0)).xyz;
    UV = aUV;    
    Normal = mat3(transpose(inverse(M))) * aNormal;//生成法线矩阵
}
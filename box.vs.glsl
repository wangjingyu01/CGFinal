#version 330 core
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aColor;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;

//out vec3 vertexColor;
//out vec3 vColor;
//out vec2 uv;
out vec3 Normal;
out vec3 posWS;//world position ->WS
out vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
//	vColor = aColor;
    posWS=(model*vec4(aPos,1.0)).xyz;
    UV=aUv;
    Normal = mat3(transpose(inverse(model))) * aNormal;//生成法线矩阵
}
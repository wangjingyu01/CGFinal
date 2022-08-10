#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

/*第一个参数：纹理图片路径
第二个参数：第id个纹理（0开始）
第三个参数：默认填data*/
void	texture(const char* texpath, int id, unsigned char* data)
{
	unsigned int texture;
	int width, height, nrChannels;
	glGenTextures(1, &texture);
	stbi_set_flip_vertically_on_load(TRUE);
	glActiveTexture(GL_TEXTURE0 + id);//激活 纹理单元
	glBindTexture(GL_TEXTURE_2D, texture);//绑定texture 2D
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	data = stbi_load(texpath, &width, &height, &nrChannels, 0);
	if (data)
	{
		string a=texpath;
		if (a.find("png") == string::npos)//不是png格式
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else//倒数第三个参数不一样，搞了我好久。。。
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture：" << texpath << std::endl;
	}
	stbi_image_free(data);
}
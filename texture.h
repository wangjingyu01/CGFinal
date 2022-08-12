#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

/*第一个参数：纹理贴图路径
第二个参数：第id个纹理（0开始）*/
void	texture(const char* texpath, int id)
{
	unsigned int texture;
	int width, height, nrChannels;
	static unsigned char* data;//贴图数据指针

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
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		std::cout << "导入成功：" << texpath << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture：" << texpath << std::endl;
	}
	stbi_image_free(data);
}
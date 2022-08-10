#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

/*��һ������������ͼƬ·��
�ڶ�����������id������0��ʼ��
������������Ĭ����data*/
void	texture(const char* texpath, int id, unsigned char* data)
{
	unsigned int texture;
	int width, height, nrChannels;
	glGenTextures(1, &texture);
	stbi_set_flip_vertically_on_load(TRUE);
	glActiveTexture(GL_TEXTURE0 + id);//���� ����Ԫ
	glBindTexture(GL_TEXTURE_2D, texture);//��texture 2D
	// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// ���ز���������
	data = stbi_load(texpath, &width, &height, &nrChannels, 0);
	if (data)
	{
		string a=texpath;
		if (a.find("png") == string::npos)//����png��ʽ
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else//����������������һ���������Һþá�����
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture��" << texpath << std::endl;
	}
	stbi_image_free(data);
}
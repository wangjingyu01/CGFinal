#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//�����Լ���װ��һЩ��
#include"camera.h"
#include"model.h"
#include"shader.h"
#include"texture.h"
#include"vertex.h"

//(���ߣ�����)
GLFWwindow* glInit(const int screenWidth, const int screenHeight, const char* title)
{
	glfwInit();
	//ָ���汾 OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		//return break;
	}
	return window;
}
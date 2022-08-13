#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include"FileSystem.h"
#include"Mesh.h"
#include"camera.h"
#include"model.h"
#include"shader_s.h"
#include"ImportData.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


const char* glsl_version = "#version 130";//OpenGL版本号

//(宽，高，标题)
GLFWwindow* glInit(const int screenWidth, const int screenHeight, const char* title)
{
	glfwInit();
	//指定版本 OpenGL 3.3
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

void InitGui(GLFWwindow* window)
{
	//创建imgui上下文
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	//设置样式：常规
	ImGui::StyleColorsLight();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\simhei.ttf", 18.0f,
		NULL, io.Fonts->GetGlyphRangesChineseFull());//添加中文字体，防止显示不出中文
	//imgui设置平台和渲染
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}
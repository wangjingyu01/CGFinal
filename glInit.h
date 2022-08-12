#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//加上自己封装的一些库
#include"camera.h"
#include"model.h"
#include"shader.h"
#include"texture.h"
#include"ImportData.h"

#define print(x) cout << x << endl
#define len(x) sizeof(x)/sizeof(x[0])
#define type(x) typeid(x).name ()
//#define printV3(x)cout<<"("<<x[0]<<","<<x[1]<<","<<x[2]<<")"
//#tdefine printV2(x)cout<<" ("<<x[0]<<","<<x[1]<<")"
#define printV3(x) cout<<" ("<<x[0]<<","<<x[1]<<","<<x[2]<<")"
#define printV2(x) cout<<x[0]<<","<<x[1]
#define printCol3(x) cout<<" ("<<x.r<<","<<x.g<<","<<x.b<<")"
#define aiVec3(x) vec3(x[0],x[1],x[2])
#define aiVec2(x) vec2(x[0],x[1])

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
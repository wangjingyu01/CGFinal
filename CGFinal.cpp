#include <iostream>
#include "glm/glm.hpp"
#include <vector>
#include "glInit.h"//这里面还包含了很多自己定义的库

using namespace std;
using namespace glm;

int main()
{
	GLFWwindow* window = glInit(sizeW()*0.7, sizeH() * 0.7, "CGFinal");

	Vertex vertex(vertices, sizeof(vertices), indices, sizeof(indices), prop, propSize);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//指定清空颜色（背景）
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwPollEvents();
		glfwSwapBuffers(window);//交换画好的缓冲，发送给window
	}
	//清理缓存
	glfwTerminate();
	return 0;
}


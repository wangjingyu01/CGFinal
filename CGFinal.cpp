#include <iostream>
#include "glm/glm.hpp"
#include <vector>
#include "glInit.h"//这里面还包含了很多自己定义的库

using namespace std;
using namespace glm;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);//鼠标位置输入
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);//滚轮缩放
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);//鼠标按键输入
void key_callback(GLFWwindow* window, int a, int b, int c, int d);//键盘输入
void framebuffer_size_callback(GLFWwindow* window, int width, int height);//改变窗口的大小时，视口也调整

bool mouseCapture = true;//是否获取鼠标

Camera camera(90, 10, 0.1);

int main()
{
	GLFWwindow* window = glInit(sizeW()*0.7, sizeH() * 0.7, "CGFinal");
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);

	Vertex vertex(vertices, sizeof(vertices), indices, sizeof(indices), prop, propSize);
	Shader shader("box.vs.glsl", "box.fs.glsl");
	texture("pic/container.jpg",0);

	while (!glfwWindowShouldClose(window))
	{
		camera.processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//指定清空颜色（背景）
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 M;//model
		mat4 V = camera.getView();//view
		mat4 P = perspective(radians(camera.fov), float(sizeW() / sizeH()), 0.1f, 100.0f);//projection

		shader.use();
		shader.setMat4("M", M);
		shader.setMat4("V", V);
		shader.setMat4("P", P);
		shader.setInt("t0", 0);//这里的0是指texture路径后面跟的第0号贴图
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT ,0);

		glfwPollEvents();
		glfwSwapBuffers(window);//交换画好的缓冲，发送给window
	}
	//清理缓存
	glfwTerminate();
	return 0;
}

//鼠标位置
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (mouseCapture)
		camera.mousePos(xpos, ypos);
}
//滚轮
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.mouseScroll(yoffset);
}
//鼠标按键
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	camera.mouse_button_callback(button, action);
}
//键盘
void key_callback(GLFWwindow* window, int a, int b, int c, int d)
{
	if ((glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS))
		if (mouseCapture)
		{
			mouseCapture = false;
		}
		else
		{
			mouseCapture = true;
		}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

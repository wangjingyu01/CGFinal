#include "glInit.h"//这里面包含了一些头文件和初始化的内容

using namespace std;
using namespace glm;

Camera camera(90, 10, 0.1);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);//鼠标位置输入
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);//滚轮缩放
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);//鼠标按键输入
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);//键盘输入
void framebuffer_size_callback(GLFWwindow* window, int width, int height);//改变窗口的大小时，视口也调整

bool mouseCapture = true;//是否能够移动鼠标
bool isDrawLight = true;//是否绘制灯光的实体
bool wireframe = false;//线框模式
vec3 lightPos = vec3(0, 7, 4);//照明位置
vec3 lightColor = vec3(1, 1, 1);//光照颜色
float lightIntancity = 5.0f;//光照强度

vec3 cameraPos = camera.cameraPos;

string modelPath_man = "Model/nanosuit/nanosuit.obj";
string modelPath_cube = "Model/cube/cube.obj";

int main()
{
	GLFWwindow* window = glInit(sizeW()*0.7, sizeH() * 0.7, "CGFinal");
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	InitGui(window);
	//stbi_set_flip_vertically_on_load(false);
	glEnable(GL_DEPTH_TEST);
	
    Shader shader("shader/box.vs.glsl", "shader/box.fs.glsl");
	Shader skyboxShader("shader/skybox.vs.glsl", "shader/skybox.fs.glsl");
	Shader lightShader("shader/lightbox.vs.glsl", "shader/lightbox.fs.glsl");
	
	Model ModelMan(modelPath_man);
	Model ModelCube(modelPath_cube);


    //变换矩阵
    mat4 V = camera.getView();//view
    mat4 P = perspective(radians(camera.fov), float(sizeW() / sizeH()), 0.1f, 100.0f);//projection

	//导入天空盒
	unsigned int skyboxVAO, skyboxVBO;
	leadinSkyBox(skyboxVAO, skyboxVBO);//自己封装的一个函数
	skyboxShader.use();
	skyboxShader.setInt("t0", 0);

	while (!glfwWindowShouldClose(window))
	{
		//创建imgui窗口
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin(u8"设置");//离谱 没这个u8显示不出中文。。。
			ImGui::Text(u8"帧率 (%.1f FPS)", ImGui::GetIO().Framerate);
			const char* ismove = u8"开";
			if (mouseCapture)
				ismove = u8"开";
			else
				ismove = u8"关";
			ImGui::SameLine();
			ImGui::Text(u8"     鼠标移动：%s （ALT键控制）", ismove);

			if (ImGui::CollapsingHeader(u8"灯光基本设置"))
			{
				ImGui::Checkbox(u8"显示点光源位置", &isDrawLight);
				ImGui::ColorEdit3(u8"光照颜色选择", (float*)&lightColor);
				ImGui::SliderFloat(u8"光照强度", &lightIntancity, 0.0f, 10.0f);
				ImGui::SliderFloat(u8"灯光位置（x水平方向上）", &lightPos.x, -10.0f, 10.0f);
				ImGui::SliderFloat(u8"灯光位置（y垂直方向上）", &lightPos.y, -10.0f, 30.0f);
				ImGui::SliderFloat(u8"灯光位置（z前后方向上）", &lightPos.z, -10.0f, 10.0f);
			}

			if (ImGui::CollapsingHeader(u8"场景设置"))
			{
				ImGui::Checkbox(u8"以线框模式显示", &wireframe);
			}


			ImGui::End();
		}

		camera.processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//指定清空颜色（背景）（有了天空盒就看不见了）
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);//线框模式

		P = perspective(radians(camera.fov), float(sizeW() / sizeH()), 0.1f, 100.0f);//projection

		//绘制天空盒
		V = mat4(mat3(camera.getView()));
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		V = glm::mat4(glm::mat3(camera.getView()));
		skyboxShader.setMat4("V", V);
		skyboxShader.setMat4("P", P);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); 


		//指定参数 绘制场景
		V = camera.getView();//view
		shader.use();
		mat4 M=mat4(1);
		//设置light属性
		shader.setVec3("l.position", lightPos);
		shader.setVec3("l.ambient", vec3(0));
		shader.setVec3("l.color", lightColor);//光照颜色
		shader.setVec3("l.specular", lightColor);//物体上的高光颜色
		shader.setFloat("l.lightIntancity", lightIntancity);//光照强度

		//设置material材质属性
		shader.setVec3("m.emissive", vec3(0));
		shader.setVec3("m.diffuse", vec3(1));
		shader.setVec3("m.specular", vec3(1));
		shader.setFloat("m.shininess", 128);
		shader.setFloat("m.specularIntancity", 3);//高光强度

		shader.setVec3("cameraPos", camera.cameraPos);
		shader.setMat4("M", M);
		shader.setMat4("V", V);
		shader.setMat4("P", P);
		ModelMan.Draw(shader);



		lightShader.use();
		M = mat4(1);
		M = translate(M, lightPos);
		M = scale(M, vec3(0.2));
		lightShader.setMat4("M", M);
		lightShader.setMat4("V", V);
		lightShader.setMat4("P", P);
		lightShader.setFloat("lightIntancity", lightIntancity);
		lightShader.setVec3("lightColor", lightColor);
		if(isDrawLight)
			ModelCube.Draw(lightShader);

		//结束绘制

		ImGui::Render();		//imgui渲染
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);//交换画好的缓冲，发送给window
		glfwPollEvents();
	}
	//清理缓存
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ((key== GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) || (key == GLFW_KEY_RIGHT_ALT && action == GLFW_PRESS))
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

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
int LightNum = 3;//设置点光源的数量
int lightControl = 0;//在控制第几盏灯

bool isDirectionalLight = false;//是否启用平行光
bool isPointLight = true;//是否启用点光源
bool isSpotlight = false;//是否启用聚光灯

vec3 DirectionalLightPos = vec3(1, 1, 1);//平行光方向
vec3 DirectionalLightColor = vec3(1, 1, 1);//平行光颜色
vec3 PointlightPos[] = { vec3(5,15,5) ,vec3(3,7,3),vec3(-6,5,3),vec3(-3,13,3),vec3(3,13,-10) };//点光源位置
vec3 PointlightColor[] = { vec3(1, 1, 1),vec3(0.8, 0.8, 0),vec3(1, 0.5, 0.5),vec3(1, 0, 1),vec3(1, 1, 1), };//点光源颜色
float PointlightIntancity[] = { 4 ,3 ,3.5 ,2.5 ,4.5 };//点光源光照强度
float SpotLightIntancity = 1;


vec3 cameraPos = camera.cameraPos;

string modelPath_man = "Model/nanosuit/nanosuit.obj";
string modelPath_cube = "Model/cube/cube.obj";

int main()
{
	GLFWwindow* window = glInit(sizeW() * 0.9, sizeH() * 0.9, "CGFinal");
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
				ImGui::Checkbox(u8"启用点光源", &isPointLight);
				ImGui::Checkbox(u8"启用平行光", &isDirectionalLight);
				ImGui::Checkbox(u8"启用聚光灯", &isSpotlight);

				if (isPointLight && ImGui::TreeNode(u8"点光源设置"))
				{
					ImGui::Checkbox(u8"显示点光源位置", &isDrawLight);
					{//设置点光源数量
						if (ImGui::Button(u8"增加点光源") && LightNum <= 4)
							LightNum++;
						ImGui::SameLine();
						if (ImGui::Button(u8"减少点光源") && LightNum > 1)
							LightNum--;
						ImGui::SameLine();
						ImGui::Text(u8"光源数： %d", LightNum);
					}
					//有几盏灯就有几个选项
					if (LightNum >= 1) ImGui::RadioButton(u8"控制第一盏灯", &lightControl, 0);
					if (LightNum >= 2) ImGui::RadioButton(u8"控制第二盏灯", &lightControl, 1);
					if (LightNum >= 3) ImGui::RadioButton(u8"控制第三盏灯", &lightControl, 2);
					if (LightNum >= 4) ImGui::RadioButton(u8"控制第四盏灯", &lightControl, 3);
					if (LightNum >= 5) ImGui::RadioButton(u8"控制第五盏灯", &lightControl, 4);

					ImGui::ColorEdit3(u8"光照颜色选择", (float*)&PointlightColor[lightControl]);
					ImGui::SliderFloat(u8"光照强度", &PointlightIntancity[lightControl], 0.0f, 10.0f);
					ImGui::SliderFloat(u8"灯光位置（x水平方向上）", &PointlightPos[lightControl].x, -10.0f, 10.0f);
					ImGui::SliderFloat(u8"灯光位置（y垂直方向上）", &PointlightPos[lightControl].y, -10.0f, 30.0f);
					ImGui::SliderFloat(u8"灯光位置（z前后方向上）", &PointlightPos[lightControl].z, -10.0f, 10.0f);
				}
				if (isDirectionalLight && ImGui::TreeNode(u8"平行光设置"))
				{
					ImGui::ColorEdit3(u8"灯光颜色", (float*)&DirectionalLightColor); 
					ImGui::SliderFloat(u8"灯光方向（x）", &DirectionalLightPos.x, -1, 1);
					ImGui::SliderFloat(u8"灯光方向（y）", &DirectionalLightPos.y, -1, 1);
					ImGui::SliderFloat(u8"灯光方向（z）", &DirectionalLightPos.z, -1, 1);
				}
				if (isSpotlight && ImGui::TreeNode(u8"聚光灯设置"))
					ImGui::SliderFloat(u8"光照强度", &SpotLightIntancity, 0, 5);
			}

			if (ImGui::CollapsingHeader(u8"场景设置"))
			{
				ImGui::Checkbox(u8"以线框模式显示", &wireframe);
				ImGui::Checkbox(u8"漫游模式（空格键控制）", &camera.wanderMode);
			}

			ImGui::End();
		}

		camera.processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//指定清空颜色（背景）（有了天空盒就看不见了）
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);//线框模式
		if (camera.wanderMode)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//隐藏鼠标
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//显示鼠标
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
		shader.setBool("isPointLight", isPointLight);
		shader.setBool("isDirectionalLight", isDirectionalLight);
		shader.setBool("isSpotlight", isSpotlight);

		mat4 M = mat4(1);

		//设置 平行光 DirectionalLight属性
		if (isDirectionalLight)//开启平行光，就给参数
		{
			shader.setVec3("dirLight.direction", -DirectionalLightPos);
			shader.setVec3("dirLight.color", DirectionalLightColor);
		}
		else//不开就给0
		{
			shader.setVec3("dirLight.direction", vec3(0, 0, 0));
			shader.setVec3("dirLight.color", vec3(0, 0, 0));
		}

		//设置 点光源 PointLight属性

		shader.setInt("LightNum", LightNum);
		if (isPointLight)
		{
			for (int i = 0; i < LightNum; i++)
			{
				string strpos, strcol, strint, strspe, stri;
				stri = to_string(i);
				strpos = "pointLight[" + stri + "].position";
				strspe = "pointLight[" + stri + "].specular";
				strcol = "pointLight[" + stri + "].color";
				strint = "pointLight[" + stri + "].lightIntancity";
				shader.setVec3(strpos, PointlightPos[i]);
				shader.setVec3(strspe, PointlightColor[i]);
				shader.setVec3(strcol, PointlightColor[i]);
				shader.setFloat(strint, PointlightIntancity[i]);
			}
		}
		else
		{
			for (int i = 0; i < LightNum; i++)
			{
				string strpos, strcol, strint, strspe, stri;
				stri = to_string(i);
				strpos = "pointLight[" + stri + "].position";
				strspe = "pointLight[" + stri + "].specular";
				strcol = "pointLight[" + stri + "].color";
				strint = "pointLight[" + stri + "].lightIntancity";
				shader.setVec3(strpos, vec3(0, 0, 0));
				shader.setVec3(strspe, vec3(0, 0, 0));
				shader.setVec3(strcol, vec3(0, 0, 0));
				shader.setFloat(strint, 0);
			}
		}

		//设置 聚光灯 SpotLight属性
		if (isSpotlight)
		{
			shader.setVec3("spotLight.position", camera.cameraPos);
			shader.setVec3("spotLight.direction", camera.cameraFront);
			shader.setVec3("spotLight.color", vec3(1, 232.0f / 255.0f, 0));
			shader.setFloat("spotLight.lightIntancity", SpotLightIntancity);
		}
		else
		{
			shader.setVec3("spotLight.color", vec3(0));
		}



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

		//画表示灯光的盒子
		for (int i = 0; i < LightNum; i++)
		{
			lightShader.use();
			M = mat4(1);
			M = translate(M, PointlightPos[i]);
			M = scale(M, vec3(0.2));
			lightShader.setMat4("M", M);
			lightShader.setMat4("V", V);
			lightShader.setMat4("P", P);
			lightShader.setFloat("lightIntancity", PointlightIntancity[i]);
			lightShader.setVec3("lightColor", PointlightColor[i]);
			if (isDrawLight && isPointLight)
				ModelCube.Draw(lightShader);
		}


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
	if (mouseCapture)
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
	if ((key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) || (key == GLFW_KEY_RIGHT_ALT && action == GLFW_PRESS))
		mouseCapture = !mouseCapture;
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		camera.wanderMode = !camera.wanderMode;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}







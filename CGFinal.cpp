#include <iostream>
#include "glm/glm.hpp"
#include <vector>
#include "glInit.h"//这里面还包含了很多自己定义的库


using namespace std;
using namespace glm;

struct MeshIndices
{
    unsigned int vID;
    unsigned int offset;
    unsigned int matID;//材质ID
};
struct Material
{
    string diff;//颜色贴图
    string spec;//高光贴图
    string norm;//法线贴图
};
vector <Material> mats;
vector <string> textures;
vector<MeshIndices> Meshes_OK;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);//鼠标位置输入
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);//滚轮缩放
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);//鼠标按键输入
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);//键盘输入
void framebuffer_size_callback(GLFWwindow* window, int width, int height);//改变窗口的大小时，视口也调整
void Draw(MeshIndices meshes, Shader shader,mat4 M,vec3 transform = vec3(0, 0, 0),
                float angle = 0.0f, vec3 axis = vec3(1, 0, 0), vec3 scaleSize = vec3(1, 1, 1));//绘制
void Import(const std::string& pFile);//导入模型

bool mouseCapture = true;//是否获取鼠标

Camera camera(90, 10, 0.1);



int main()
{
	GLFWwindow* window = glInit(sizeW()*0.7, sizeH() * 0.7, "CGFinal");
	glEnable(GL_DEPTH_TEST);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Import("Model/ball/prism2-2.obj");

    //批量导入贴图
    for (int i = 0; i < textures.size(); i++)
        texture(textures[i].c_str(), i);

    unsigned char* data;
    Shader shader("box.vs.glsl", "box.fs.glsl");

    //创建物体
    MeshIndices box0 = Meshes_OK[0];
    //变换矩阵
    mat4 V = camera.getView();//view
    mat4 P = perspective(radians(camera.fov), float(sizeW() / sizeH()), 0.1f, 100.0f);//projection

	while (!glfwWindowShouldClose(window))
	{
		camera.processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//指定清空颜色（背景）
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 M;//model
        V = camera.getView();//view
        P = perspective(radians(camera.fov), float(sizeW() / sizeH()), 0.1f, 100.0f);//projection

		shader.use();
        shader.setMat4("V", V);
        shader.setMat4("P", P);

        for (int i = 0; i < Meshes_OK.size(); i++)
        {
            shader.setInt("t0", 1);
            Draw(Meshes_OK[i], shader, M,vec3 (i*5,0,0));
        }


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

//绘制
/*第1个参数：
第2个：
第3个：变换矩阵M（model）
第4个：变换矩阵V（view）
第5个：变换矩阵P（projection）
第6个：位置
第7个：旋转角度
第8个：旋转轴
第9个：缩放大小
*/
void Draw(MeshIndices meshes, Shader shader,
	mat4 M, vec3 transform , float angle, vec3 axis , vec3 scaleSize )
{
	M = translate(M, transform);
	M = rotate(M, radians(angle), axis);
	M = scale(M, scaleSize);
	shader.setMat4("M", M);

	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(meshes.vID), GL_UNSIGNED_INT, (void*)(meshes.offset * sizeof(float)));
}

int getTexID(vector<string> v, string a)//通过文件的路径找到在vector中的位置
{
    vector<string>::iterator it = find(v.begin(), v.end(), a);//查找是否有字符a
    if (it != v.end())
        return std::distance(v.begin(), it);
    else
        return -1;
}
void Import(const std::string& pFile)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(pFile,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

    if (!scene)
    {
        cout << importer.GetErrorString();
    }

    int aiMeshes = scene->mNumMeshes;

    vector <Vertex> vertices;
    vector <unsigned int> indices;

    int SumVertices = 0;//解决多个物体的indices都是从0开始
    int SumIndices = 0;

    //获取所有mesh 名
    for (int i = 0; i < aiMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[i];
        cout << mesh->mName.C_Str()<< " : \n" << "顶点数量="<< mesh->mNumVertices;

        for (int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertex.Pos = vec3(mesh->mVertices[i][0], mesh->mVertices[i][1], mesh->mVertices[i][2]);//填充Pos
            vertex.Normal = vec3(mesh->mNormals[i][0], mesh->mNormals[i][1], mesh->mNormals[i][2]);//填充Normal
            vertex.UV = vec2(mesh->mTextureCoords[0][i][0], mesh->mTextureCoords[0][i][1]);//填充UV
            vertices.push_back(vertex);
        }

        //获取面ID
        int faces = mesh->mNumFaces;
        MeshIndices temp_m;
        temp_m.vID = faces * 3;
        temp_m.offset = SumIndices;
        temp_m.matID = mesh->mMaterialIndex;
        Meshes_OK.push_back(temp_m);
        for (int i = 0; i < faces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j] + SumVertices);
        }
        cout << "=========================" << endl;
        SumVertices += mesh->mNumVertices;
        SumIndices += faces * 3;
    }

    int prop[] = { 3,3,2 };//属性各部分的大小
    int propSize = sizeof(prop) / sizeof(prop[0]);//总共有几个属性
    cout << "____________________" << endl;
    cout << "vertices数量：" << vertices.size() << endl;
    cout << "indices数量：" << indices.size() << endl;

    ImportData(vertices, indices, prop, propSize);//读取顶点

    //获取贴图
    aiString texPath;
    int numMat = scene->mNumMaterials;
    aiTextureType texType[ ]={ aiTextureType_DIFFUSE ,aiTextureType_SPECULAR,aiTextureType_HEIGHT };
    string defaultTex[] = { "pic/white.jpg","pic/black.jpg", "pic/normal.jpg" };
    string* p;
    for (int i = 0; i < numMat; i++)
    {
        aiMaterial* mat = scene->mMaterials[i];
        Material tempMat;//临时的mat结构体
        p = &tempMat.diff;
        for (int j = 0; j < len(texType); j++)
        {
            texPath = "";
            mat->GetTexture(texType[j], 0, &texPath);
            string tex = texPath.C_Str();
            if (texPath.length > 0)
            {
                (*p++) = tex;
                textures.push_back(tex);
            }
            else
                (*p++) = defaultTex[j];
        }
        mats.push_back(tempMat);
    }

    //vector去重
    vector <string>::iterator it;
    sort(textures.begin(), textures.end());
    it = unique(textures.begin(), textures.end());
    if (it != textures.end())
        textures.erase(it, textures.end());

    for (int i = 0; i < sizeof(defaultTex) / sizeof(defaultTex[0]); i++) 
        textures.insert(textures.begin(),defaultTex[sizeof(defaultTex) / sizeof(defaultTex[0])-i-1]);
    for (int i = 0; i < textures.size(); i++) 
        print(textures[i]);
}

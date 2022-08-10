#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Vertex {
public:
    int sumProp = 0;
    /*第一个参数：顶点数组（填vertices）
    第二个参数：第一个参数的大小（填sizeof(vertices)）
    第三个参数：索引数组（填indices）
    第四个参数：索引数组大小（填sizeof(indsize)）
    第五个参数：顶点数组每部分有几个数（填prop）
    第六个参数：顶点数组有几个部分（填propSize)*/
	Vertex(float vertices[],int versise, int indices[],int indsize, int prop[], int propSize)
	{
        //创建VAO VBO EBO
        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        //绑定
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, versise, vertices, GL_STATIC_DRAW);//第三个参数是希望画出的数据
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indsize, indices, GL_STATIC_DRAW);

        int* propOffset = new int[propSize];
        int tem=0;
        for (int i = 0; i < propSize; i++)
        {
            sumProp += prop[i];
            if (i == 0)
                propOffset[i] = 0;
            else
                tem += prop[i - 1];
            propOffset [i] = tem;
        }
        for (int i = 0; i < propSize; i++)
        {
            if (i == 0)
                setProperty(i, prop[i], sumProp, 0);
            else
                setProperty(i, prop[i], sumProp, propOffset[i]);
        }

	}
   
    /*第一个参数：Id=0、1、2、3按顺序排下来
第二个参数：第Id个参数由几个浮点数组成
第三个参数：这Id个参数总共有几个浮点数
第四个参数：第Id个参数的偏移量*/
    void setProperty(int Id, int pSize, int vSize, int Offset)

    {
        glVertexAttribPointer(Id, pSize, GL_FLOAT, GL_FALSE, vSize * sizeof(float), (void*)(Offset * sizeof(float)));
        glEnableVertexAttribArray(Id);
    }
};
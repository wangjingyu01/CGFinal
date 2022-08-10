#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Vertex {
public:
    int sumProp = 0;
    /*��һ���������������飨��vertices��
    �ڶ�����������һ�������Ĵ�С����sizeof(vertices)��
    �������������������飨��indices��
    ���ĸ����������������С����sizeof(indsize)��
    �������������������ÿ�����м���������prop��
    ���������������������м������֣���propSize)*/
	Vertex(float vertices[],int versise, int indices[],int indsize, int prop[], int propSize)
	{
        //����VAO VBO EBO
        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        //��
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, versise, vertices, GL_STATIC_DRAW);//������������ϣ������������
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
   
    /*��һ��������Id=0��1��2��3��˳��������
�ڶ�����������Id�������ɼ������������
��������������Id�������ܹ��м���������
���ĸ���������Id��������ƫ����*/
    void setProperty(int Id, int pSize, int vSize, int Offset)

    {
        glVertexAttribPointer(Id, pSize, GL_FLOAT, GL_FALSE, vSize * sizeof(float), (void*)(Offset * sizeof(float)));
        glEnableVertexAttribArray(Id);
    }
};
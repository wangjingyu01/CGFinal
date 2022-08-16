#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include"model.h"


class ImportData {
public:
    int sumProp = 0;
    /*��һ���������������飨��vertices��
    �ڶ����������������飨��indices��
    ��������������������ÿ�����м���������prop��
    ���ĸ����������������м������֣���propSize)*/
    ImportData(vector <Vertex> vertices, vector <unsigned int> indices, int prop[], int propSize)
	{
        //����VAO VBO EBO
        unsigned int VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        //��
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);//������������ϣ������������
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

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


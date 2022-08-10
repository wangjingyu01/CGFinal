    #pragma once

#include <glad/glad.h>;
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

string loadShaderSrc(const char* filePath);

class Shader
{
public:
    Shader(const char* vs, const char* fs) 
    {
        this->vs = vs;
        this->fs = fs;

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);//����shader
        std::string vertShaderSrc = loadShaderSrc(vs);//��ȡgLsl ��vertShaderSrc
        const GLchar* vertShader = vertShaderSrc.c_str();//��stringת��ΪGLchar
        glShaderSource(vertexShader, 1, &vertShader, NULL);//����source
        glCompileShader(vertexShader);//����shader


        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        std::string fragShaderSrc = loadShaderSrc(fs);
        const GLchar* fragShader = fragShaderSrc.c_str();
        glShaderSource(fragmentShader, 1, &fragShader, NULL);
        glCompileShader(fragmentShader);


        //����fs vs
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        //ɾ��shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    //��ȡ Shader �ļ�
    string loadShaderSrc(const char* filePath)
    {
        ifstream file; // �����ļ�
        stringstream buf;//�����ļ�buf
        string ret = "";//��ȡ�ļ���ret

        file.open(filePath);

        if (file.is_open()) {
            buf << file.rdbuf();//���ļ���ȡ��buf
            ret = buf.str(); //��buf����ַ���ret
        }
        else
            cout << "Could not open " << filePath << endl;
        file.close();//�ر��ļ�
        return ret;
        }

    void use()
    {
        glUseProgram(shaderProgram);
    }

       void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); 
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); 
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, &value[0]); 
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w); 
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }


    const char* vs;
    const char* fs;
    unsigned int shaderProgram;

};


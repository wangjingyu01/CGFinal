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

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);//创建shader
        std::string vertShaderSrc = loadShaderSrc(vs);//读取gLsl 到vertShaderSrc
        const GLchar* vertShader = vertShaderSrc.c_str();//把string转换为GLchar
        glShaderSource(vertexShader, 1, &vertShader, NULL);//设置source
        glCompileShader(vertexShader);//编译shader


        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        std::string fragShaderSrc = loadShaderSrc(fs);
        const GLchar* fragShader = fragShaderSrc.c_str();
        glShaderSource(fragmentShader, 1, &fragShader, NULL);
        glCompileShader(fragmentShader);


        //链接fs vs
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        //删除shader
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    //读取 Shader 文件
    string loadShaderSrc(const char* filePath)
    {
        ifstream file; // 创建文件
        stringstream buf;//创建文件buf
        string ret = "";//读取文件到ret

        file.open(filePath);

        if (file.is_open()) {
            buf << file.rdbuf();//把文件读取到buf
            ret = buf.str(); //把buf里的字符给ret
        }
        else
            cout << "Could not open " << filePath << endl;
        file.close();//关闭文件
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


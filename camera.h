#pragma once

#include<Windows.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

int sizeW();
int sizeH();

class Camera
{

public:
    //�����Ұ������ٶȡ���������ȡ����λ��
    Camera(float fov = 45.0f, float speed = 2.5f, float sensitivity = 0.05f, vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f)) //�����Ұ������ٶȡ����������

    {
        this->fov = fov;
        this->speed = speed;
        this->sensitivity = sensitivity;
        this->cameraPos = cameraPos;
    }

    float fov;
    float speed;
    float sensitivity;
    vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f);
    vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
    bool wanderMode = false;//����ģʽ

    //��������
    void processInput(GLFWwindow* window)
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        float cameraSpeed = speed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += normalize(cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            cameraPos.y += cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            cameraPos.y -= cameraSpeed;            
    }

    //���λ������
    void mousePos(double xpos, double ypos)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        if (!wanderMode)//��������ģʽ��
        {
            if (MouseDown)//ֻ����갴��ʱ����ת��������������ӽǻ�����һ��ת
                ProcessMouseMovement(xoffset, yoffset);
        }
        else
            ProcessMouseMovement(xoffset, yoffset);
    }

    //����������룬�������߽Ƕȣ�ƫ���Ǻ͸�����
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        Yaw += xoffset;
        Pitch += yoffset;
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        updateCameraVectors();//�����������
    }

    //��������
    void mouseScroll(double yoffset)
    {
        if (fov >= 15.0f && fov <= 85.0f)
            fov -= yoffset;
        if (fov <= 15.0f)
            fov = 15.0f;
        if (fov >= 85.0f)
            fov = 85.0f;
    }

    void mouse_button_callback(int button, int action)
    {

        if (action == GLFW_PRESS)
            switch (button)
            {
            case GLFW_MOUSE_BUTTON_LEFT:	//�����ѡ����ק
                MouseDown = true;
            }
        else MouseDown = false;
        return;
    }


    mat4 getView()
    {
        return lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

private:
    vec3 cameraUp = vec3(0.0f, 1.0f, 0.0f);
    float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
    float lastFrame = 0.0f; // ��һ֡��ʱ��

    float Pitch = 0.0f;
    float Yaw = -90.0f;
    float lastX = sizeW() / 2.0;
    float lastY = sizeH() / 2.0;
    bool firstMouse = true;
    bool MouseDown;    // ��¼���������¼�

     //�����������
    void updateCameraVectors() {
        vec3 front;
        front.x = cos(radians(Yaw)) * cos(radians(Pitch));
        front.y = sin(radians(Pitch));
        front.z = sin(radians(Yaw)) * cos(radians(Pitch));
        cameraFront = normalize(front);
    }
};

int sizeW()
{
    //�����Ļ�ֱ��ʣ����䲻ͬ��Ļ
    DEVMODE dm;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
    int SCR_WIDTH = dm.dmPelsWidth;
    return SCR_WIDTH;
}
int sizeH()
{
    //�����Ļ�ֱ��ʣ����䲻ͬ��Ļ
    DEVMODE dm;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
    int SCR_HEIGHT = dm.dmPelsHeight;
    return SCR_HEIGHT;
}

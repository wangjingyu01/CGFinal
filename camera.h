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
    //相机视野、相机速度、鼠标灵敏度、相机位置
    Camera(float fov = 45.0f, float speed = 2.5f, float sensitivity = 0.05f, vec3 cameraPos = vec3(0.0f, 0.0f, 3.0f)) //相机视野、相机速度、鼠标灵敏度

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
    bool wanderMode = false;//游览模式

    //键盘输入
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

    //鼠标位置输入
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

        if (!wanderMode)//不在游览模式下
        {
            if (MouseDown)//只有鼠标按下时才能转动摄像机，否则视角会和鼠标一起转
                ProcessMouseMovement(xoffset, yoffset);
        }
        else
            ProcessMouseMovement(xoffset, yoffset);
    }

    //根据鼠标输入，更新视线角度，偏航角和俯仰角
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
        updateCameraVectors();//更新相机属性
    }

    //滚轮缩放
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
            case GLFW_MOUSE_BUTTON_LEFT:	//左键：选择拖拽
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
    float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f; // 上一帧的时间

    float Pitch = 0.0f;
    float Yaw = -90.0f;
    float lastX = sizeW() / 2.0;
    float lastY = sizeH() / 2.0;
    bool firstMouse = true;
    bool MouseDown;    // 记录鼠标点击相关事件

     //更新相机属性
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
    //检测屏幕分辨率，适配不同屏幕
    DEVMODE dm;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
    int SCR_WIDTH = dm.dmPelsWidth;
    return SCR_WIDTH;
}
int sizeH()
{
    //检测屏幕分辨率，适配不同屏幕
    DEVMODE dm;
    EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
    int SCR_HEIGHT = dm.dmPelsHeight;
    return SCR_HEIGHT;
}

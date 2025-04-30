#ifndef CAMERA_H
#define CAMERA_H



#include <bits/stdc++.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;


class Camera{
    bool firstmove=true;
    float fov=50.0f;
    vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
    GLFWwindow* inputWindow;

    public:
        mat4 viewMatrix;
        mat4 projectionMatrix;
        vec3 position = vec3(0.0f, 0.0f, 3.0f);
        vec3 upDir = vec3(0.0f, 1.0f, 0.0f);
        float speed=0.05f;
        float yaw=-90.0f;
        float pitch=0.0f;

        Camera(float fov){
            this->fov=fov;
            this->viewMatrix=glm::lookAt(position,position+cameraFront,upDir);
            this->projectionMatrix = perspective(radians(fov), 800.0f/600.0f, 0.1f, 100.0f);
            this->cameraFront =vec3(cos(radians(yaw)) * cos(radians(pitch)),
                                    sin(radians(pitch)),
                                    sin(radians(yaw)) * cos(radians(pitch)));
            
        }

        void lookAt(vec3 target){
            this->viewMatrix = glm::lookAt(position,target,upDir);
        }

        void setInputWindow(GLFWwindow* window){
            this->inputWindow = window;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, this->mouse_callback);
            // Set this Camera instance as the user pointer for the window
            glfwSetWindowUserPointer(window, this);
        }
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos){
            Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
            static float lastX=400;
            static float lastY=300;
            if(cam->firstmove){
                lastX=xpos;
                lastY=ypos;
                cam->firstmove=false;
            }
            float xoffset=(xpos-lastX)*0.06;
            float yoffset=(lastY-ypos)*0.06;
            lastX=xpos;
            lastY=ypos;
            cam->yaw+=xoffset;
            cam->pitch+=yoffset;
            if(cam->pitch > 89.0f)cam->pitch = 89.0f;
            if(cam->pitch < -89.0f)cam->pitch = -89.0f;
            cam->cameraFront =vec3(cos(radians(cam->yaw)) * cos(radians(cam->pitch)),
                                    sin(radians(cam->pitch)),
                                    sin(radians(cam->yaw)) * cos(radians(cam->pitch)));
        }
        void processInput(){
            if (glfwGetKey(inputWindow, GLFW_KEY_W) == GLFW_PRESS) 
                position += speed * cameraFront;
            if (glfwGetKey(inputWindow, GLFW_KEY_S) == GLFW_PRESS)
                position -= speed * cameraFront;
            if (glfwGetKey(inputWindow, GLFW_KEY_A) == GLFW_PRESS)
                position -= glm::normalize(glm::cross(cameraFront, upDir)) * speed;
            if (glfwGetKey(inputWindow, GLFW_KEY_D) == GLFW_PRESS)
                position += glm::normalize(glm::cross(cameraFront, upDir)) * speed;
            this->viewMatrix = glm::lookAt(position,position+cameraFront,upDir);
        }
};

#endif
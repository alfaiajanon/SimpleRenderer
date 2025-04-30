#ifndef OBJECT3D_H
#define OBJECT3D_H


#include <bits/stdc++.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <custom/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <custom/camera.h>

using namespace std;
using namespace glm;


class Object3D{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO=-1;
    mat4 modelMatrix=mat4(1.0f);
    vec3 position=vec3(0.0f,0.0f,0.0f);
    float rotationAngle=0.0f;
    vec3 rotationAxis=vec3(0.0f,0.0f,1.0f);
    void update(){
        mat4 model=mat4(1.0f);
        model=translate(model, position);
        model=rotate(model,radians(rotationAngle),rotationAxis);
        modelMatrix=model;
    }

    public:
        Shader shader;
        Object3D(unsigned int VAO, unsigned int VBO, unsigned int EBO, Shader shaderProgram){
            this->VAO=VAO;
            this->VBO=VBO;
            this->EBO=EBO;
            this->shader=shaderProgram;
            customFunc=[](vector<float> data){
                //do nothing
            };
        }

        vector<float> customData;
        function<void(vector<float>)> customFunc;

        void setPosition(float x, float y, float z){
            position=vec3(x,y,z);
            update();
        }

        void setRotation(float angle, vec3 axis){
            rotationAngle=angle;
            rotationAxis=axis;
            update();
        }

        void render(Camera camera){
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            shader.use();
            shader.setMatrix4("model",modelMatrix);
            shader.setMatrix4("view",camera.viewMatrix);
            shader.setMatrix4("projection", camera.projectionMatrix);
            shader.setVec3("viewPos",camera.position);
            customFunc(customData);
            if(EBO==-1)
                glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
        }
};

#endif
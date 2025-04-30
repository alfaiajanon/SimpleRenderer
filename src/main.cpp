#include <bits/stdc++.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <custom/shader.h>
#include <custom/camera.h>
#include <custom/object3d.h>
#include <custom/Model.h>
#include <custom/Light.h>

using namespace std;
using namespace glm;


void _framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}




int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple Renderer", NULL, NULL);
    if(window == NULL ){
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, _framebuffer_size_callback);
    glClearColor(.02f,.08f,.1f,1.0f);
    glEnable(GL_DEPTH_TEST);



    vector<Model> models;
    Model model("./src/backpack/backpack.obj");
    model.setPosition(2,0,-4);
    models.push_back(model);
    
    Light light(vec3(1,1,1));



    Shader standardShader("./src/simpleVertexShader.vs",
                          "./src/simpleFragmentShader.fs");

    standardShader.use();
    standardShader.setFloat("material.shininess", 32.0f);
    standardShader.setVec3("light.ambient", light.ambient());
    standardShader.setVec3("light.diffuse", light.diffuse());
    standardShader.setVec3("light.specular", light.diffuse());
    standardShader.setVec3("light.position",light.pos());



    Camera camera = Camera(65);
    camera.setInputWindow(window);


    
    while(!glfwWindowShouldClose(window)){ 
        camera.processInput();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        for(Model obj:models){
            obj.render(camera,standardShader);
        }
        light.render(camera,standardShader);

        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    return 0;
}





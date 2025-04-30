#ifndef MESH_H
#define MESH_H

#include <bits/stdc++.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <custom/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

struct Vertex{
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
};

struct Texture{
    unsigned int id;
    string type;
    string path;
};

class Mesh{
    public:
        //mesh data
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        mat4 modelMatrix=mat4(1.0f);

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures){
            this->vertices=vertices;
            this->indices=indices;
            this->textures=textures;
            setupMesh();
        }
        void render(Camera cam,Shader shader){
            glBindVertexArray(VAO);
            shader.use();
            int diffuseNr = 1;
            int specularNr = 1;
            for(unsigned int i = 0; i < textures.size(); i++){
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, textures[i].id);

                string number;
                string name = textures[i].type;
                if(name == "texture_diffuse")
                    number = to_string(diffuseNr++);
                else if(name == "texture_specular")
                    number = to_string(specularNr++);
                shader.setInt(("material." + name + number).c_str(), i);
            }
            glActiveTexture(GL_TEXTURE0);

            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }


    private:
        unsigned int VAO, VBO, EBO;
        void setupMesh(){
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float)));
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            glBindVertexArray(0);
        }
};

#endif
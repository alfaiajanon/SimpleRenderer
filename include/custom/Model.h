#ifndef MODEL_H
#define MODEL_H

#include <bits/stdc++.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <custom/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <custom/Mesh.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <image/stb_image.h>

using namespace std;
using namespace glm;
using namespace Assimp;


class Model{
    public:
        int unshaded=0;
        Model(){
            
        }
        Model(string path){
            loadModel(path);
        }
        
        void setPosition(float x, float y, float z){
            position=vec3(x,y,z);
            update();
        }

        void setRotation(float angle, vec3 axis){
            rotationAngle=angle;
            rotationAxis=axis;
            update();
        }

        void setScale(float x, float y, float z){
            scale=vec3(x,y,z);
            update();
        }

        void render(Camera cam, Shader shader){
            for(Mesh &m : meshes){
                shader.use();
                shader.setMatrix4("model",modelMatrix);
                shader.setMatrix4("view",cam.viewMatrix);
                shader.setMatrix4("projection", cam.projectionMatrix);
                shader.setVec3("viewPos",cam.position);
                shader.setInt("unshaded",unshaded);
                m.render(cam,shader);
            }
        }

        unsigned int TextureFromFile(string path, int mode){
            unsigned int texture;
            glGenTextures(1, &texture);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            int width, height, nrChannels;
            stbi_set_flip_vertically_on_load(true);
            unsigned char* image = stbi_load(path.c_str(), &width,&height,&nrChannels,0);
            if(!image){
                cout << "Failed to load image" << endl;
                cout <<  path << endl;
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, mode, GL_UNSIGNED_BYTE, image);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(image);
            cout<<texture<<" "<<path<<endl;
            return texture;
        }
        
    


        //attributes
        vector<Mesh> meshes;
    private:
        string directory;
        vector<Texture> textures_loaded;
        mat4 modelMatrix=mat4(1.0f);
        vec3 position=vec3(0.0f,0.0f,0.0f);
        float rotationAngle=0.0f;
        vec3 rotationAxis=vec3(0.0f,0.0f,1.0f);
        vec3 scale=vec3(1.0,1.0,1.0);

        void update(){
            mat4 model=mat4(1.0f);
            model=glm::scale(model,scale);
            model=rotate(model,radians(rotationAngle),rotationAxis);
            model=translate(model, position);
            modelMatrix=model;
        }

        void loadModel(string path){
            Importer importer;
            const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
                cout<<"Error : "<<importer.GetErrorString()<<endl;
                return;
            }
            directory=path.substr(0,path.find_last_of('/'));
            processNode(scene->mRootNode, scene);
        }


        void processNode(aiNode *node, const aiScene *scene){
            for(int i=0; i<node->mNumMeshes; i++){
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }
            for(int i=0; i<node->mNumChildren; i++)
                processNode(node->mChildren[i],scene);
        }

        
        Mesh processMesh(aiMesh *mesh, const aiScene *scene){
            vector<Vertex> vertices;
            vector<unsigned int> indices;
            vector<Texture> textures;
            for(int i=0; i<mesh->mNumVertices; i++){
                Vertex vertex;
                vertex.Position=vec3(mesh->mVertices[i].x,
                                     mesh->mVertices[i].y,
                                     mesh->mVertices[i].z);
                vertex.Normal=vec3(mesh->mNormals[i].x,
                                   mesh->mNormals[i].y,
                                   mesh->mNormals[i].z);
                if(mesh->mTextureCoords[0])
                    vertex.TexCoords=vec2(mesh->mTextureCoords[0][i].x,
                                          mesh->mTextureCoords[0][i].y);
                else
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);
                vertices.push_back(vertex);
            }
            for(unsigned int i = 0; i < mesh->mNumFaces; i++){
                aiFace face = mesh->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            if(mesh->mMaterialIndex >= 0){
                aiMaterial *material=scene->mMaterials[mesh->mMaterialIndex];
                vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
                vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            }
            return Mesh(vertices, indices, textures);
        }


        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName){
            vector<Texture> textures;
            for(int i=0; i<mat->GetTextureCount(type); i++){
                aiString str;
                mat->GetTexture(type, i, &str);

                bool skip=false;
                for(Texture t:textures_loaded){
                    if(t.path == str.C_Str()){
                        textures.push_back(t);
                        skip=true;
                        break;
                    }
                }
                if(!skip){
                    Texture texture;
                    texture.id= TextureFromFile(directory+"/"+str.C_Str(),GL_RGB);
                    texture.type=typeName;
                    texture.path=str.C_Str();
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);
                }
            }
            return textures;
        }

        
        
};

#endif
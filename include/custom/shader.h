#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <bits/stdc++.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Shader{
    public:
        unsigned int ID;
        Shader(){}
        Shader(const char* vertexPath, const char* fragmentPath){
            string vertexCode;
            string fragmentCode;
            ifstream vShaderFile;
            ifstream fShaderFile;

            vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
            fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
            try{
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                stringstream vShaderStream, fShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }catch(ifstream::failure e){
                cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
                return;
            }
            

            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();
            unsigned int vertex, fragment;
            vertex=glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            fragment=glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment,1,&fShaderCode,NULL);
            glCompileShader(fragment);

            ID=glCreateProgram();
            glAttachShader(ID,vertex);
            glAttachShader(ID,fragment);
            glLinkProgram(ID);
            cout<<"shader created "<<ID<<endl;

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use(){
            glUseProgram(ID);
        }

        void setBool(const std::string &name, bool value) const{
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        }
        void setInt(const std::string &name, int value) const{
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string &name, float value) const{
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setMatrix4(const std::string &name, mat4 value) const{
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value_ptr(value));
        }
        void setVec3(const std::string &name, vec3 vec) const{
            this->setVec3(name,vec.x,vec.y,vec.z);
        }
        void setVec3(const std::string &name, float x,float y, float z) const{
            glUniform3f(glGetUniformLocation(ID,name.c_str()),x,y,z);
        }

        unsigned int get(string s){
            return glGetUniformLocation(ID,s.c_str());
        }
};


#endif
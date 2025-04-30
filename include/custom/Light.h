#ifndef LIGHT_H
#define LIGHT_H

#include <bits/stdc++.h>
#include <custom/Model.h>

class Light{
    vec3 color;
    float ambientStrength=0.2f;
    vec3 position=vec3(0,0,0);
    Model lightModel=Model();
    void makeModel(){
        vector<Vertex> vertices;
        for(int i=0;i<4;i++){
            Vertex v;
            float x=i>1?-1:1;
            float y=abs(1.5-i)>0.5?1:-1;
            v.Position=vec3(x,y,0);
            v.Normal=vec3(0,0,1);
            v.TexCoords=vec2(0.5+0.5*x, 0.5+0.5*y);
            vertices.push_back(v);
            // cout<<v.TexCoords.x<<" "<<v.TexCoords.y<<endl;
        }
        vector<unsigned int> indices{0,1,2,0,2,3};
        vector<Texture> textures(2);
        textures[0].id=lightModel.TextureFromFile("./src/dot.png",GL_RGBA);
        textures[0].type="texture_diffuse";
        textures[0].path="";
        textures[1].id=lightModel.TextureFromFile("./src/dot.png",GL_RGBA);
        textures[1].type="texture_specular";
        textures[1].path="";
        Mesh m(vertices,indices,textures);
        lightModel.meshes.push_back(m);
        lightModel.unshaded=1;
        lightModel.setScale(.6,.6,.6);
    }

    public:
        Light(vec3 color){
            this->color=color;
            makeModel();
        }

        void render(Camera cam, Shader shader){
            vec3 lightDir = vec3(0,0,1);
            float angle = acos(dot(lightDir,normalize(cam.position)));
            float sign = ((cam.position-position).x < 0) ? -1.0f : 1.0f;
            lightModel.setRotation(sign*degrees(angle), vec3(0,1,0));
            lightModel.render(cam,shader);
        }

        vec3 ambient(){
            return color*ambientStrength;
        }
        vec3 diffuse(){
            return color;
        }
        Model model(){
            return lightModel;
        }
        vec3 pos(){
            return position;
        }
};


#endif
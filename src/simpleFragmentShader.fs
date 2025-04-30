#version 330 core

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
    float     shininess;
};
struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform int unshaded;

in vec2 texCord;
in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;


void main(){
    if(texture(material.texture_diffuse1,texCord).a<0.1)
        discard;
    if(unshaded==1){
        FragColor=texture(material.texture_diffuse1,texCord);
        return;
    }

    
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, texCord));

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCord));

    vec3 viewDir = normalize(viewPos-fragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float specFactor = pow(max(dot(viewDir,reflectDir),0.0), 32);
    vec3 specular = specFactor * light.specular * vec3(texture(material.texture_specular1,texCord));

    vec3 result = ambient +diffuse + specular;

    FragColor = vec4(result, 1.0);
}
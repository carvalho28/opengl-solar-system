#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec3 LightPos;
in vec2 TexCoords;
  
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float shininessStrength;

uniform sampler2D texture_diffuse1;

void main()
{
    // ambient 
    //float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * lightColor) * diffuseStrength;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininessStrength);
    vec3 specular = specularStrength * spec * lightColor;  
            
    vec3 result = (ambient + diffuse + specular) * objectColor;
    // FragColor = texture(result, TexCoords);
    FragColor = texture(texture_diffuse1, TexCoords) * vec4(result, 1.0);
} 
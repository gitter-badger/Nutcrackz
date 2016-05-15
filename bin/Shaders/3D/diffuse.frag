#version 330 core

in vec2 TexCoords;
in vec3 Normals;

out vec4 FragColor;

struct DirectionalLight
{
    vec3 Color;
    float AmbientIntensity;
	float DiffuseIntensity;                                                         
    vec3 Direction;
};

uniform DirectionalLight gDirectionalLight;
uniform sampler2D texture_diffuse1;

void main()
{
	vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.AmbientIntensity;                         
                                                                                    
    float DiffuseFactor = dot(normalize(Normals), -gDirectionalLight.Direction);    
                                                                                    
    vec4 DiffuseColor;                                                              
                                                                                    
    if (DiffuseFactor > 0) {                                                        
        DiffuseColor = vec4(gDirectionalLight.Color, 1.0f) *  gDirectionalLight.DiffuseIntensity * DiffuseFactor;                                               
    }                                                                               
    else {                                                                          
        DiffuseColor = vec4(0, 0, 0, 0);                                            
    }                                                                               
                                                                                    
    FragColor = texture2D(texture_diffuse1, TexCoords.xy) * (AmbientColor + DiffuseColor);
}

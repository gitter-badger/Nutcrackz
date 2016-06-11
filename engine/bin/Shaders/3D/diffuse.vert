#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in ivec4 boneIDs;
layout (location = 4) in vec4 weights;

const int MAX_BONES = 100;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 gWorld;
uniform mat4 gBones[MAX_BONES];

out vec2 TexCoords;
out vec3 Normals;

void main()
{
	mat4 BoneTransform = gBones[boneIDs[0]] * weights[0];
    BoneTransform     += gBones[boneIDs[1]] * weights[1];
    BoneTransform     += gBones[boneIDs[2]] * weights[2];
    BoneTransform     += gBones[boneIDs[3]] * weights[3];

	vec4 PosL = BoneTransform * vec4(position, 1.0);
    gl_Position = projection * view * model * PosL;
    TexCoords = texCoords;
	vec4 NormalL = BoneTransform * vec4(normal, 0.0);
	Normals = (gWorld * NormalL).xyz;//(gWorld * vec4(normal, 0.0f)).xyz;
	WorldPos = (gWorld * PosL).xyz;

    //gl_Position = projection * view * model * vec4(position, 1.0f);
    //TexCoords = texCoords;
	//Normals = (gWorld * vec4(normal, 0.0f)).xyz;
}

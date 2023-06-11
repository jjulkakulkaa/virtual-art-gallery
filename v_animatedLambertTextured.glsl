#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;


uniform vec4 lightDir=vec4(0,0,1,0);

//Atrybuty
layout (location=0) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location=1) in vec4 normal; //wektor normalny w wierzcholku
layout (location=2) in vec2 texCoord; //wsp?rz?dne teksturowania
layout (location=3) in ivec4 boneIds;
layout (location=4) in vec4 weights;


//Zmienne interpolowane
out vec2 i_tc;
out float i_nl;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main(void) {
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >=MAX_BONES) 
        {
            totalPosition = vertex;
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vertex;
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal.xyz;
    }

    gl_Position=P*V*M*totalPosition;

    mat4 G=mat4(inverse(transpose(mat3(M))));
    vec4 n=normalize(V*G*normal);

    i_nl=clamp(dot(n,lightDir),0,1);
    i_tc=texCoord;
}
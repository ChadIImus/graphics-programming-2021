#version 330 core

in VS_OUT {
    vec3 CamPos_tangent;    // tangent space camera position
    vec3 Pos_tangent;       // tangent space position
    vec4 Pos_lightSpace;    // light space position
    vec3 LightDir_tangent;  // tangent space light direction
    vec3 Norm_tangent;      // tangent space normal
    vec2 textCoord;         // uv texture coordinate
    mat3 invTBN;            // matrix tha rotates from TBN to worls space
} fs_in;

// light uniform variables

// skybox texture
uniform samplerCube skybox;

// shadow map uniforms

// output color
out vec4 FragColor;

vec3 InteriorMapping(vec2 texCoords, vec3 viewDir)
{
    vec3 Id = 1/viewDir;
    vec3 pos = vec3(texCoords, 1);
    vec3 k = abs(Id) - (Id * pos);
    float kMin = min(min(k.x, k.y), k.z);

    return viewDir * kMin + pos;
}

void main()
{
    vec3 viewDir = normalize(fs_in.CamPos_tangent - fs_in.Pos_tangent);
    vec3 texCoords = InteriorMapping(fs_in.textCoord, viewDir);

    FragColor = texture(skybox, texCoords);
}
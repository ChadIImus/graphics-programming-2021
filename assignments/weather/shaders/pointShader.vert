#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;


uniform float drawDistance;
uniform float lineLength;
uniform vec3 velocity;
uniform vec3 offsets;
uniform vec3 cameraPosition;
uniform vec3 prevCameraPosition;
uniform mat4 model;
uniform mat4 prevModel;

out vec4 vtxColor;

void main()
{
    vec3 position = mod(pos + offsets, drawDistance);
    position += cameraPosition - drawDistance/2;


    vec3 camDif = (cameraPosition - prevCameraPosition) * 0.9;

    vec3 prevPos = position + velocity + lineLength;

    //reduce difference between the top and bottom of the raindrop
    // by reducing x,z movement for the top of the line dependent
    //on the difference between current and previous camera pos
    prevPos = vec3(prevPos.x - camDif.x, prevPos.y, prevPos.z - camDif.z);

    //projections
    vec4 screenPos = model * vec4(position,1.0);
    vec4 prevScreenPos = prevModel * vec4(prevPos,1.0);

    //top and bottom
    if (gl_VertexID % 2 == 0){
        gl_Position = prevScreenPos;
    } else {
        gl_Position = screenPos;
    }

    //longer raindrops = faster moving camera
    //the faster the camera moves, the more the rain should be invisible (motion blur)
    vtxColor = vec4(color.xyz,1.0-abs(distance(vec3(screenPos.x, screenPos.y, screenPos.z), vec3(prevScreenPos.x, prevScreenPos.y, prevScreenPos.z))));
}
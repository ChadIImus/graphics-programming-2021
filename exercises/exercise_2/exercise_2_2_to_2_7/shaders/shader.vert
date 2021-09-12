#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 velocity;
layout (location = 2) in float timeOfBirth;
// the position variable has attribute position 0
uniform float currentTime;
out float age;
void main()
{
    age = currentTime - timeOfBirth;
    if(timeOfBirth == 0){
        gl_Position = vec4(2, 2,0, 1.0);

    } else if (age < 10.0){

        // this is the output position and and point size (this time we are rendering points, instad of triangles!)
        gl_Position = vec4(pos + (velocity * age), 0.0, 1.0);
    } else {
        gl_Position = vec4(2, 2,0, 1.0);
    }
    gl_PointSize = mix(0.1,20.0, age/10) * 5;
}
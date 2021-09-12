#version 330 core

out vec4 fragColor;

in float age;
void main()
{
    // TODO 2.6: improve the particles appearance

    float alpha = (1 - abs(sqrt(pow((gl_PointCoord.x - 0.5),2) + pow((gl_PointCoord.y - 0.5),2))) * 2 ) - age/10;

    // remember to replace the default output (vec4(1.0,1.0,1.0,1.0)) with the color and alpha values that you have computed
    if(age <= 5.0){
        float g = mix(1.0,0.5,age/10);
        float b = mix(0.5,0.1,age/10);
        fragColor = vec4(1.0, g, b, alpha);
    }else{
        float r = mix(1.0,0.0,age/10);
        float g = mix(0.5,0.0,age/10);
        float b = mix(0.1,0.0,age/10);
        fragColor = vec4(r, g, b, alpha);
    }

}
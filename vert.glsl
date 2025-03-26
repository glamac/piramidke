#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
uniform float time;
out vec3 vertCol;
out vec2 TexCoord;
mat3 rotate3dy(float _angle) {
    return mat3(
        cos(_angle),0,sin(_angle),
        0,1,0,
        -sin(_angle),0,cos(_angle)
    );
}
float pi = 3.14159;
void main()
{
    vec3 nPos = aPos;
    vertCol = aColor;
    nPos *= rotate3dy(time);
    float angle = -pi/5;
    nPos *= mat3(
        1, 0, 0,
        0, cos(angle), -sin(angle),
        0, sin(angle), cos(angle)
    );
    gl_Position = vec4(nPos, 1.0f);
    TexCoord = aTexCoord;
}
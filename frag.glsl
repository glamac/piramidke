#version 330 core
in vec3 vertCol;
in vec2 TexCoord;
out vec4 FragColor;
uniform float time;
uniform sampler2D texture1;
void main()
{
    //FragColor = vec4(vertCol, 1.0f);
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0f);
    FragColor = texture(texture1, TexCoord);
}

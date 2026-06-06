#version 330 compatibility

uniform mat4 u_mvp;

out vec4 f_color;
out vec2 f_texCoord;

void main()
{
    gl_Position = u_mvp * gl_Vertex;
    f_color = gl_Color;
    f_texCoord = gl_MultiTexCoord0.st;
}

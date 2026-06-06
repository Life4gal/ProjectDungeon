#version 330 compatibility

uniform sampler2D u_texture;
uniform vec4 u_start_color;
uniform vec4 u_end_color;
uniform float u_ratio;

in vec4 f_color;
in vec2 f_texCoord;

void main()
{
    vec4 tex_color = texture2D(u_texture, f_texCoord);
    vec4 color = mix(u_start_color, u_end_color, u_ratio);
    gl_FragColor = tex_color * color;
}

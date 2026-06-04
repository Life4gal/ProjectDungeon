uniform sampler2D u_texture;
uniform vec4 u_start_color;
uniform vec4 u_end_color;
uniform float u_ratio;

void main()
{
    vec4 tex_color = texture2D(u_texture, gl_TexCoord[0].st);
    vec4 color = mix(u_start_color, u_end_color, u_ratio);
    gl_FragColor = tex_color * color;
}

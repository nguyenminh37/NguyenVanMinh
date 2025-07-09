#version 300 es
//precision mediump float;
//layout(location = 0) out mediump vec4 o_color;
//in mediump vec3 v_color;

layout(location=0) out mediump vec4 o_color;
uniform sampler2D u_texture;
in mediump vec2 v_uv;
void main()
{
	//o_color = vec4(1.0, 0.0, 0.0, 1.0);
       //o_color= vec4(v_color, 1.0) ;

o_color = texture(u_texture, v_uv);

}


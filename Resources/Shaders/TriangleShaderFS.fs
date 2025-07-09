#version 300 es
precision mediump float;
layout(location = 0) out mediump vec4 o_color;
in mediump vec3 v_color;
void main()
{
	//o_color = vec4(1.0, 0.0, 0.0, 1.0);
        o_color= vec4(v_color, 1.0) ;
}

#version 300 es
layout(location=0) in highp vec4 a_position;
layout(location=1) in mediump vec2 a_uv;
out mediump vec2 v_uv;
void main()
{ 
	gl_Position = a_position;
	v_uv = a_uv;
}
   

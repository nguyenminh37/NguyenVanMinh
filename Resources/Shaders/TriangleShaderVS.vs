#version 300 es
layout(location=0) in highp vec3 a_position;
layout(location=1) in mediump vec2 a_uv;
out mediump vec2 v_uv;
void main(){ 
	gl_Position = vec4(a_position, 1.0);
	v_uv = a_uv;
}
   

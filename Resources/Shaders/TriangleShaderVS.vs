#version 300 es
//layout(location=0) in highp vec3 a_position;
//layout(location=1) in mediump vec2 a_uv;

layout (location = 0) in highp vec3 a_position;
layout (location = 1) in highp vec3 a_normal;
layout (location = 2) in highp vec3 a_binormal;
layout (location = 3) in highp vec3 a_tangent;
layout (location = 4) in mediump vec2 a_uv; // UV ? location 4

out mediump vec2 v_uv;
void main(){ 
	gl_Position = vec4(a_position, 2.0);
	v_uv = a_uv;
}

 
   

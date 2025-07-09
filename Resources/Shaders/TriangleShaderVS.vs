#version 300 es

   
layout(location=0) in highp vec3 a_position; 
layout(location=1) in mediump vec3 a_color;

out mediump vec3 v_color;
void main() 
{ 

gl_Position = vec4(a_position, 1.0);
v_color = a_color;
}
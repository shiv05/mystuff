#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_ms;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal_ms;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 vertexPosition_ws;
out vec3 Normal_cs;
out vec3 EyeDirection_cs;
out vec3 LightDirection_cs[10];

// Values that stay constant for the whole mesh.
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 NormalMatrix;
uniform int num_lights_used;
uniform float lightpower[10];
uniform vec3 lightpos_ws[10];
uniform vec3 lightcolor[10];

void main(){
	
	mat4 MVP = P * V * M;
	gl_Position =  MVP * vec4(vertexPosition_ms,1);

	vertexPosition_ws = (M * vec4(vertexPosition_ms,1)).xyz;
	vec3 vertexPosition_cs = ( V * M * vec4(vertexPosition_ms,1)).xyz;
	EyeDirection_cs = vec3(0,0,0) - vertexPosition_cs;

	for(int i=0;i<num_lights_used;i++)
	{
		vec3 LightPosition_cs = ( V * vec4(lightpos_ws[i],1)).xyz;
		//vec3 LightPosition_cs = LightPosition_ws;
		LightDirection_cs[i] = LightPosition_cs - vertexPosition_cs;
	}
	
	Normal_cs = (NormalMatrix * vec4(vertexNormal_ms,0)).xyz; 
	
	UV = vertexUV;
}
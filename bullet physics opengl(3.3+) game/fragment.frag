#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 vertexPosition_ws;
in vec3 Normal_cs;
in vec3 EyeDirection_cs;
in vec3 LightDirection_cs[10];

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
//uniform sampler2D myTextureSampler;
//uniform mat4 MV;
uniform vec3 mat_amb;
uniform vec3 mat_diff;
uniform vec3 mat_spec;
uniform float mat_shin;

uniform int num_lights_used;
uniform vec3 lightpos_ws[10];
uniform vec3 lightcolor[10];
uniform float lightpower[10];



void main(){

	vec3 n = normalize( Normal_cs );
	vec3 E = normalize(EyeDirection_cs);

	color = mat_amb;

	for(int i = 0;i<num_lights_used;i++)
	{
		float distance = length( lightpos_ws[i]- vertexPosition_ws );
		vec3 l = normalize( LightDirection_cs[i] );
		float cosTheta = clamp( dot( n,l ), 0,1 );
		vec3 R = reflect(-l,n);
		float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	

		color+= (mat_diff * lightcolor[i] * lightpower[i] * cosTheta  + 
		mat_spec * lightcolor[i]  * lightpower[i] * pow(cosAlpha,mat_shin))/(distance * distance);
	}
}
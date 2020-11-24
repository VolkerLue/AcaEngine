#version 450

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUv;

uniform mat4 Transform;
uniform vec3 lightPos;
uniform vec3 viewerPos;

out vec2 uv;
out vec3 Normal;
out vec3 lpos;


void main() {

    uv= vertexUv;
	
	gl_Position = Transform * vec4(vertexPosition, 2);
	
	Normal = normalize(vertexNormal);


	lpos = normalize(vertexPosition - lightPos);
	
}
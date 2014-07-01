#version 120

uniform mat4 modelViewProjectionMatrix;

void main(){
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}

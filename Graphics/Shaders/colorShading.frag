#version 130


in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

uniform float  time;


void main(){
	color = vec4(fragmentColor.r * (cos(fragmentPosition.x * 4.0 + time) + 1.0) * 0.5,
				fragmentColor.g * (cos(fragmentPosition.y * 8.0 + time) + 1.0) * 0.5,
				fragmentColor.b * (cos(fragmentPosition.x * 2.0 + time) + 1.0) * 0.5, fragmentColor.a);
	//vec4(1.0 * (cos(time)+1.0) * 0.5, 1.0 * (cos(time + 2.0)+1.0) * 0.5, 1.0 * (sin(time + 1.0)+1.0) * 0.5, 0.0);
}
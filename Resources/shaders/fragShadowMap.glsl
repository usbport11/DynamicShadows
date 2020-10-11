#ifdef GL_ES                                                                         
precision lowp float;                                                                
#endif 

#define PI 3.14

varying vec2 v_texCoord;
varying vec4 v_color;

uniform sampler2D u_texture;
uniform vec2 resolution;

//for debugging; use a constant value in final release
uniform float upScale;
uniform float accuracy;

//alpha threshold for our occlusion map
const float THRESHOLD = 0.99;

void main(void) {
	float distance = 1.0;
	float theta = PI * 1.5 + (v_texCoord.s * 2.0 - 1.0) * PI;
	float add = accuracy / resolution.y;
	vec2 preCoord = vec2(sin(theta), cos(theta)) * 0.5;
	for (float r = 0.0; r < 1.0; r += add) {
		vec2 coord = -r * preCoord + 0.5;
		vec4 data = texture2D(u_texture, coord);
		if (data.a > THRESHOLD) {
			distance = r;
			break;
		}
	}
	gl_FragColor = vec4(distance / upScale, 0.0, 0.0, 1.0);
}

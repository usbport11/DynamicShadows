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
	for (float y = 0.0; y < resolution.y; y += accuracy) {
		vec2 norm = vec2(v_texCoord.s, y / resolution.y) * 2.0 - 1.0;
		float theta = PI * 1.5 + norm.x * PI;
		float r = (1.0 + norm.y) * 0.5;
		vec2 coord = vec2(-r * sin(theta), -r * cos(theta)) / 2.0 + 0.5;
		vec4 data = texture2D(u_texture, coord);
		float dst = y / resolution.y / upScale;
		float caster = data.a;
		if (caster > THRESHOLD) {
			distance = min(distance, dst);
		}
	}
	gl_FragColor = vec4(vec3(distance), 1.0);
}
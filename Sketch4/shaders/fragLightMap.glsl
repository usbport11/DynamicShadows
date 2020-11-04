varying vec2 v_texCoord;
varying vec4 v_color;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform vec4 ambientColor;
uniform vec2 resolution;

void main() {
	vec4 diffuseColor = texture(u_texture1, v_texCoord);
	vec2 lightCoord = (gl_FragCoord.xy / resolution.xy);
	vec4 light = texture(u_texture2, lightCoord);
	
	vec3 ambient = (ambientColor.rgb * ambientColor.a) + light.rgb;
	vec3 final = diffuseColor.rgb * ambient;
	
	gl_FragColor = vec4(final, diffuseColor.a);
}
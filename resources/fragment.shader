#version 330

uniform float time;
uniform vec2 resolution;
uniform vec4 color;
uniform sampler2D bloomTexture;

out vec4 FragColor;

void main() {
	vec2 uv = gl_FragCoord.xy / resolution;
    vec4 newColor = vec4(color.xyz / 255.0, 1.0);

    float scaler = 12.0;
    float x = abs(sin(uv.x * time * scaler));
    FragColor = vec4(mix(vec3(x, uv.y, 0.0), newColor.rgb, sin(gl_FragCoord.y + gl_FragCoord.x)), 1.0);
}
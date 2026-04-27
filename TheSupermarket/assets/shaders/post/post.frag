#version 330 core
in  vec2      TexCoord;
out vec4      FragColor;
uniform sampler2D screenTexture;
uniform float     fearFactor;
uniform float     time;
uniform float     chaseFactor;
uniform float     slipFactor;
uniform float     jumpscareT;

void main() {
    vec2 uv = TexCoord;

    float aberr = fearFactor * 0.015 + chaseFactor * 0.025;
    float r = texture(screenTexture, uv + vec2( aberr, 0.0)).r;
    float g = texture(screenTexture, uv).g;
    float b = texture(screenTexture, uv - vec2( aberr, 0.0)).b;

    float slipShift = sin(time * 12.0) * slipFactor * 0.03;
    uv.x += slipShift;
    uv.y += cos(time * 10.0) * slipFactor * 0.02;

    uv.x += sin(uv.y * 15.0 + time * 6.0) * chaseFactor * 0.005;

    vec2  vigUV = uv * (1.0 - uv.yx);
    float vig   = pow(clamp(vigUV.x * vigUV.y * 15.0, 0.0, 1.0),
                      0.25 + fearFactor * 0.6 + chaseFactor * 0.8);

    float noise = fract(sin(dot(uv + time * 0.001,
                    vec2(127.1, 311.7))) * 43758.5453);

    vec3 color = vec3(r, g, b);
    color *= vig;
    color += (noise - 0.5) * 0.03 * fearFactor;
    color -= sin(uv.y * 900.0) * 0.03 * fearFactor;

    vec3 oilSheen = vec3(0.9, 0.95, 1.1) * slipFactor * 0.25;
    color += oilSheen;

    color = mix(color, vec3(1.0, 0.95, 0.9), jumpscareT * 0.9);

    FragColor = vec4(clamp(color, 0.0, 1.0), 1.0);
}

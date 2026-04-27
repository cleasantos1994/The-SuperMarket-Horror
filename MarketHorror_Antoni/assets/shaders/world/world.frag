#version 330 core
in vec2  TexCoord;
in vec3  FragPos;
in vec3  Normal;
out vec4 FragColor;
uniform sampler2D texture1;
uniform vec3      lightPos;
uniform vec3      viewPos;
uniform float     time;
uniform float     fearFactor;
uniform float     chaseFactor;

void main() {
    vec2 uv = TexCoord;
    uv.x += sin(uv.y * 8.0 + time * 4.0) * fearFactor * 0.02;

    vec4  texColor = texture(texture1, uv);
    vec3  norm     = normalize(Normal);
    vec3  lightDir = normalize(lightPos - FragPos);
    float diff     = max(dot(norm, lightDir), 0.2);

    vec3  chaseRed = mix(vec3(1.0), vec3(1.5, 0.3, 0.3), chaseFactor * 0.7);
    vec3  color    = texColor.rgb * diff * chaseRed;
    float lum      = dot(color, vec3(0.299, 0.587, 0.114));
    color = mix(color, vec3(lum * 0.6), fearFactor * 0.45);

    FragColor = vec4(color, texColor.a);
}

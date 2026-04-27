#version 330 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;
out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
uniform mat4 model, view, projection;
uniform float time;
uniform float fearFactor;
void main() {
    vec3 pos = aPos;
    pos.x += sin(pos.y * 3.0 + time * 1.5) * fearFactor * 0.025;
    pos.y += cos(pos.x * 2.0 + time * 1.2) * fearFactor * 0.015;
    FragPos  = vec3(model * vec4(pos, 1.0));
    Normal   = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

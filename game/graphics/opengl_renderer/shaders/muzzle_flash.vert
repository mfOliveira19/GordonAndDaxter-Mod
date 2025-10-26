#version 410 core

layout (location = 0) in vec2 position_in;
layout (location = 1) in vec2 uv_in;

out vec2 tex_coord;

void main() {
    gl_Position = vec4(position_in, 0, 1.0);
    tex_coord = uv_in;
}

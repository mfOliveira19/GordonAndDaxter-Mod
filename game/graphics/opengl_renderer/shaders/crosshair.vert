#version 410 core
layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_uv;

out vec2 v_uv;

uniform vec2 u_resolution;

void main() {
    vec2 ndc = (a_pos / u_resolution) * 2.0 - 1.0;
    ndc.y = -ndc.y; // flip Y if needed
    gl_Position = vec4(ndc, 0.0, 1.0);
    v_uv = a_uv;
}

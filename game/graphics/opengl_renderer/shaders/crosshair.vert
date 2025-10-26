#version 410 core

layout (location = 0) in vec2 in_pos;

uniform vec2 u_resolution;
uniform vec4 u_color;

out vec4 v_color;

void main() {
  vec2 clip = (in_pos / u_resolution) * 2.0 - 1.0;
  clip.y = -clip.y;
  gl_Position = vec4(clip, 0.0, 1.0);
  v_color = u_color;
}

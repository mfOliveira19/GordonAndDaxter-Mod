#version 410 core

uniform sampler2D tex_T0;
out vec4 color;
in vec2 tex_coord;

void main() {
  vec4 texColor = texture(tex_T0, tex_coord);
  color = vec4(texColor.rgb, texColor.a * 0.5);
}

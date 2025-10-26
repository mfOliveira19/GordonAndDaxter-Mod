#include "Crosshair.h"

#include <filesystem>

#include "common/log/log.h"

#include "third-party/imgui/imgui.h"

Crosshair::Crosshair() {
}

Crosshair::~Crosshair() {

}

void Crosshair::render(DmaFollower& dma,
                       SharedRenderState* render_state,
                       ScopedProfilerNode& prof,
                       CrosshairDebugStats* debug_stats) {

  while (dma.current_tag_offset() != render_state->next_bucket)
    dma.read_and_advance();

  draw_crosshair(render_state);
  debug_stats->num_draws = 1;

}

void Crosshair::draw_crosshair(SharedRenderState* render_state) {
  if (get_type() == CrosshairType::None)
    return;

  auto& shader = render_state->shaders[ShaderId::CROSSHAIR];
  GLuint prog = shader.id();
  if (prog == 0) {
    lg::error("[crosshair] shader not compiled/linked!");
    return;
  }

  shader.activate();

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  int width = render_state->draw_region_w;
  int height = render_state->draw_region_h;

  glUniform2f(glGetUniformLocation(prog, "u_resolution"), (float)width, (float)height);
  glUniform4f(glGetUniformLocation(prog, "u_color"), 255.0f / 255.0f, 154.0f / 255.0f,
              0.0f / 255.0f, 1.0f);

  float cx = width * 0.5f;
  float cy = height * 0.5f;

  std::vector<float> verts;

  const float reference_height = 1080.0f;
  float scale_factor = (float)height / reference_height;
  scale_factor = std::clamp(scale_factor, 0.75f, 1.5f);

  if (get_type() == CrosshairType::Pistol) {
    float gap = 4.5f * scale_factor;
    float len = 4.0f * scale_factor;

    verts = {
        cx - gap - len,
        cy,
        cx - gap,
        cy,
        cx + gap,
        cy,
        cx + gap + len,
        cy,

        cx,
        cy - gap - len,
        cx,
        cy - gap,
        cx,
        cy + gap,
        cx,
        cy + gap + len,
    };

  } else if (get_type() == CrosshairType::SMG) {
    float gap = 15.0f * scale_factor;
    float len = 1.5f * scale_factor;
    float center_size = 0.5f * scale_factor;

    verts = {
        cx - gap - len,
        cy,
        cx - gap,
        cy,
        cx + gap,
        cy,
        cx + gap + len,
        cy,

        cx,
        cy - gap - len,
        cx,
        cy - gap,
        cx,
        cy + gap,
        cx,
        cy + gap + len,

        cx - center_size,
        cy,
        cx + center_size,
        cy,
        cx,
        cy - center_size,
        cx,
        cy + center_size,
    };
  }

  GLuint vao, vbo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

  glLineWidth(std::round(2.0f * scale_factor));
  glDrawArrays(GL_LINES, 0, (GLsizei)(verts.size() / 2));

  glBindVertexArray(0);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
}

void Crosshair::draw_debug_window(CrosshairDebugStats* debug_stats) {
  if (!ImGui::CollapsingHeader("Crosshair Debug"))
    return;

  ImGui::Text("Draws: %d", debug_stats->num_draws);
  ImGui::Text("Predicted Draws: %d", debug_stats->num_predicted_draws);
}

#include "Crosshair.h"

#include <filesystem>

#include "common/log/log.h"
#include "third-party/stb_image/stb_image.h"
#include "third-party/imgui/imgui.h"

Crosshair::Crosshair() {
  pistol_1280.tex = load_texture("crosshair_1280_pistol.png", pistol_1280.width, pistol_1280.height);
  smg_1280.tex = load_texture("crosshair_1280_smg.png", smg_1280.width, smg_1280.height);
  pistol_2560.tex = load_texture("crosshair_2560_pistol.png", pistol_2560.width, pistol_2560.height);
  smg_2560.tex = load_texture("crosshair_2560_smg.png", smg_2560.width, smg_2560.height);
}

Crosshair::~Crosshair() {

}

void Crosshair::render(DmaFollower& dma,
                       SharedRenderState* render_state,
                       ScopedProfilerNode& prof,
                       CrosshairDebugStats* debug_stats) {

  while (dma.current_tag_offset() != render_state->next_bucket)
    dma.read_and_advance();

  if (!crosshairShow()) {
    return;
  }

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

  int width = Gfx::g_global_settings.game_res_w;
  int height = Gfx::g_global_settings.game_res_h;

  glUniform2f(glGetUniformLocation(prog, "u_resolution"), (float)width, (float)height);
  glUniform4f(glGetUniformLocation(prog, "u_color"), 1.0f, 1.0f, 1.0f, 1.0f);

  float cx = std::floor(width * 0.5f) + 0.5f;
  float cy = std::floor(height * 0.5f) + 0.5f;


  CrosshairSprite* sprite = pick_sprite(get_type(), width);
  if (!sprite || sprite->tex == 0)
    return;

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, sprite->tex);
  glUniform1i(glGetUniformLocation(prog, "u_tex"), 0);

  float scale = 0.9f;
  draw_sprite(sprite->tex, cx, cy, (float)sprite->width * scale, (float)sprite->height * scale);
}

GLuint Crosshair::load_texture(const std::string& filename, int& out_w, int& out_h) {
  std::string path =
      (file_util::get_jak_project_dir() / "custom_assets" / "jak1" / "sprites" / filename).string();

  int channels;
  unsigned char* data = stbi_load(path.c_str(), &out_w, &out_h, &channels, 4);
  if (!data) {
    lg::error("Failed to load texture: {}", path);
    return 0;
  }

  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, out_w, out_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  stbi_image_free(data);
  return tex;
}

void Crosshair::draw_sprite(GLuint tex, float cx, float cy, float w, float h) {
  glBindTexture(GL_TEXTURE_2D, tex);

  float half_w = w * 0.5f;
  float half_h = h * 0.5f;

  float verts[] = {cx - half_w, cy - half_h, 0.0f, 0.0f, cx + half_w, cy - half_h, 1.0f, 0.0f,
                   cx + half_w, cy + half_h, 1.0f, 1.0f, cx - half_w, cy + half_h, 0.0f, 1.0f};

  GLuint indices[] = {0, 1, 2, 2, 3, 0};

  GLuint vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

  glBindVertexArray(0);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glDeleteVertexArrays(1, &vao);
}

CrosshairSprite* Crosshair::pick_sprite(CrosshairType type, int screen_width) {
  bool high_res = screen_width >= 2560;
  if (type == CrosshairType::Pistol)
    return high_res ? &pistol_2560 : &pistol_1280;
  else if (type == CrosshairType::SMG)
    return high_res ? &smg_2560 : &smg_1280;

  return nullptr;
}

void Crosshair::draw_debug_window(CrosshairDebugStats* debug_stats) {
  if (!ImGui::CollapsingHeader("Crosshair Debug"))
    return;

  ImGui::Text("Draws: %d", debug_stats->num_draws);
  ImGui::Text("Predicted Draws: %d", debug_stats->num_predicted_draws);
}

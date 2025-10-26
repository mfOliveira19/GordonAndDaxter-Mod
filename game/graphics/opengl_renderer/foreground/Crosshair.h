#pragma once

#include <string>
#include <vector>

#include "common/math/Vector.h"

#include "game/graphics/opengl_renderer/BucketRenderer.h"
#include "game/graphics/opengl_renderer/Shader.h"
#include "game/graphics/opengl_renderer/opengl_utils.h"

struct CrosshairDebugStats {
  int num_draws = 0;
  int num_predicted_draws = 0;
};

enum class CrosshairType {
  None,
  Pistol,
  SMG ,
};

inline CrosshairType int_to_crosshair(int value) {
  switch (value) {
    case 1:
      return CrosshairType::Pistol;
    case 2:
      return CrosshairType::SMG;
    case 0:
    default:
      return CrosshairType::None;
  }
}

class Crosshair {
 public:
  Crosshair();
  ~Crosshair();

  void render(DmaFollower& dma,
              SharedRenderState* render_state,
              ScopedProfilerNode& prof,
              CrosshairDebugStats* debug_stats);

  void draw_debug_window(CrosshairDebugStats* debug_stats);
  CrosshairType get_type() const {
    int raw_value = Gfx::g_global_settings.current_crosshair;
    return int_to_crosshair(raw_value);
  }

 private:
  void draw_crosshair(SharedRenderState* render_state);
  CrosshairType m_type = CrosshairType::Pistol;
};

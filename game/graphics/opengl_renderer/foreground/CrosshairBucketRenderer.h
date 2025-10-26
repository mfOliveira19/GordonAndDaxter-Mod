#pragma once

#include "game/graphics/opengl_renderer/BucketRenderer.h"
#include "game/graphics/opengl_renderer/foreground/Crosshair.h"

class CrosshairBucketRenderer : public BucketRenderer {
 public:
  CrosshairBucketRenderer(const std::string& name, int my_id, std::shared_ptr<Crosshair> crosshair);
  void draw_debug_window() override;
  void render(DmaFollower& dma, SharedRenderState* render_state, ScopedProfilerNode& prof) override;
  bool empty() const override;

 private:
  bool m_empty = false;
  std::shared_ptr<Crosshair> m_renderer;
  CrosshairDebugStats m_debug_stats;
};

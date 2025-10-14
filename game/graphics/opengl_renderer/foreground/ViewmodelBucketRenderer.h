#pragma once

#include "game/graphics/opengl_renderer/BucketRenderer.h"
#include "game/graphics/opengl_renderer/foreground/Viewmodel.h"

class ViewmodelBucketRenderer : public BucketRenderer {
 public:
  ViewmodelBucketRenderer(const std::string& name, int my_id, std::shared_ptr<Viewmodel> viewmodel);
  void draw_debug_window() override;
  void render(DmaFollower& dma, SharedRenderState* render_state, ScopedProfilerNode& prof) override;
  bool empty() const override;

 private:
  bool m_empty = false;
  std::shared_ptr<Viewmodel> m_renderer;
  ViewmodelDebugStats m_debug_stats;
};

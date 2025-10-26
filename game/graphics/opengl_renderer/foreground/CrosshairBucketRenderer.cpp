#include "CrosshairBucketRenderer.h"

CrosshairBucketRenderer::CrosshairBucketRenderer(const std::string& name,
                                                 int my_id,
                                                 std::shared_ptr<Crosshair> crosshair)
    : BucketRenderer(name, my_id), m_renderer(crosshair) {}

void CrosshairBucketRenderer::render(DmaFollower& dma,
                                     SharedRenderState* render_state,
                                     ScopedProfilerNode& prof) {
  // skip if disabled
  if (!m_enabled) {
    while (dma.current_tag_offset() != render_state->next_bucket) {
      dma.read_and_advance();
    }
    return;
  }

  m_renderer->render(dma, render_state, prof, &m_debug_stats);
  m_empty = m_debug_stats.num_predicted_draws == 0;
}

void CrosshairBucketRenderer::draw_debug_window() {
  m_renderer->draw_debug_window(&m_debug_stats);
}

bool CrosshairBucketRenderer::empty() const {
  return m_empty;
}

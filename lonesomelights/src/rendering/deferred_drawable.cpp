#include "rendering/deferred_drawable.h"

#include "geometry/camera.h"
#include "rendering/opengl/render_program.h"

DeferredDrawable::DeferredDrawable(const RenderProgram& render_program, const RenderProgram& deferred_render_program)
	: Drawable(render_program),
	m_deferred_render_program(deferred_render_program) {
}

void DeferredDrawable::draw_deferred(const Camera& camera, const Texture& color_texture, const Texture& position_texture, const Texture& normal_texture, const Texture& depth_texture) const {
}
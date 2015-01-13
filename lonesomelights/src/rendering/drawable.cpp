#include "rendering/drawable.h"

#include "geometry/camera.h"
#include "rendering/opengl/render_program.h"

Drawable::Drawable(const RenderProgram& render_program)
	: m_render_program(render_program) {
}

void Drawable::draw(const Camera& camera) const {
}
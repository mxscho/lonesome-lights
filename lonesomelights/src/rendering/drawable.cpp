#include "rendering/drawable.h"

#include "geometry/camera.h"
#include "rendering/opengl/render_program.h"

void Drawable::draw(const RenderProgram& render_program, const Camera& camera) const {
}

void Drawable::prepare_draw(const RenderProgram& render_program, const Camera& camera) const {
	render_program.bind();
	camera.bind_to_render_program(render_program, "u_view_transformation", "u_projection_transformation");
}
void Drawable::finalize_draw(const RenderProgram& render_program) const {
	RenderProgram::unbind_any();
}
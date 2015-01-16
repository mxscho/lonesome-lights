#include "rendering/opengl/render_programs.h"

#include <utility>

#include "rendering/opengl/fragment_shader.h"
#include "rendering/opengl/vertex_shader.h"

const RenderProgram& RenderPrograms::get_render_program(const std::string& name) {
	if (RenderPrograms::m_render_programs.find(name) == RenderPrograms::m_render_programs.end()) {
		VertexShader vertex_shader;
		vertex_shader.load_from_file(std::string("res/shaders/") + name + std::string("_vertex.glsl"));
		FragmentShader fragment_shader;
		fragment_shader.load_from_file(std::string("res/shaders/") + name + std::string("_fragment.glsl"));
		RenderProgram render_program;
		render_program.link(vertex_shader, fragment_shader);
		
		RenderPrograms::m_render_programs.insert(std::make_pair(name, std::move(render_program)));
	}
	return RenderPrograms::m_render_programs.find(name)->second;
}

std::map<std::string, RenderProgram> RenderPrograms::m_render_programs = std::map<std::string, RenderProgram>();
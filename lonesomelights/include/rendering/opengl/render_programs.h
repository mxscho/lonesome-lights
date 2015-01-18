#ifndef __RENDERING__OPENGL__RENDER_PROGRAMS_H__
#define __RENDERING__OPENGL__RENDER_PROGRAMS_H__

#include "rendering/opengl/render_program.h"

#include <string>
#include <map>

class RenderPrograms {
public:
	static const RenderProgram& get_render_program(const std::string& vertex_shader_name, const std::string& fragment_shader_name);
	static const RenderProgram& get_render_program(const std::string& name);
private:
	static std::map<std::string, RenderProgram> m_render_programs;
};

#endif
#include "rendering/opengl/render_program.h"

#include <cassert>
#include <iostream>
#include <utility>

#include "rendering/opengl/fragment_shader.h"
#include "rendering/opengl/vertex_shader.h"

void RenderProgram::unbind_any() {
	glUseProgram(0);
}

RenderProgram::RenderProgram()
	: m_is_generated(false),
	m_id() {
}
RenderProgram::RenderProgram(RenderProgram&& render_program)
	: m_is_generated(std::move(render_program.m_is_generated)),
	m_id(std::move(render_program.m_id)) {
	render_program.m_is_generated = false;
}
RenderProgram& RenderProgram::operator=(RenderProgram&& render_program) {
	std::swap(m_is_generated, render_program.m_is_generated);
	std::swap(m_id, render_program.m_id);
	return *this;
}
RenderProgram::~RenderProgram() {
	destroy();
}

GLuint RenderProgram::get_id() const {
	return m_id;
}

bool RenderProgram::link(const VertexShader& vertex_shader, const FragmentShader& fragment_shader) {
	destroy();

	m_id = glCreateProgram();
	glAttachShader(m_id, vertex_shader.get_id());
	glAttachShader(m_id, fragment_shader.get_id());
	glLinkProgram(m_id);

	GLint result = GL_FALSE;
	glGetProgramiv(m_id, GL_LINK_STATUS, &result);
	if (!result) {
		GLint log_length;
		glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &log_length);
		GLchar* log = new GLchar[log_length];
		glGetProgramInfoLog(m_id, log_length, &log_length, log);
		std::cerr << "Rendering: Failed to link program from shaders:" << std::endl << log << std::endl;
		delete log;
		
		m_is_generated = false;
		return false;
	}
	
	m_is_generated = true;
	return true;
}
void RenderProgram::bind() const {
	assert(m_is_generated);
	glUseProgram(m_id);
}
void RenderProgram::destroy() {
	if (m_is_generated) {
		glDeleteProgram(m_id);
		m_is_generated = false;
	}
}
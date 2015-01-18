#include "rendering/opengl/render_program.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <cassert>
#include <iostream>
#include <utility>

#include "geometry/camera.h"
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
void RenderProgram::set_uniform(const std::string& name, bool value) const {
	assert(m_is_generated);
	bind();
	
	int uniform_location = glGetUniformLocation(m_id, name.c_str());
	glUniform1ui(uniform_location, value);
}
void RenderProgram::set_uniform(const std::string& name, GLuint value) const {
	assert(m_is_generated);
	bind();
	
	int uniform_location = glGetUniformLocation(m_id, name.c_str());
	glUniform1ui(uniform_location, value);
}
void RenderProgram::set_uniform(const std::string& name, float value) const {
	assert(m_is_generated);
	bind();
	
	int uniform_location = glGetUniformLocation(m_id, name.c_str());
	glUniform1f(uniform_location, value);
}
void RenderProgram::set_uniform(const std::string& name, const glm::vec2& value) const {
	assert(m_is_generated);
	bind();
	
	int uniform_location = glGetUniformLocation(m_id, name.c_str());
	glUniform2f(uniform_location, value.x, value.y);
}
void RenderProgram::set_uniform(const std::string& name, const glm::vec3& value) const {
	assert(m_is_generated);
	bind();
	
	int uniform_location = glGetUniformLocation(m_id, name.c_str());
	glUniform3f(uniform_location, value.x, value.y, value.z);
}
void RenderProgram::set_uniform(const std::string& name, const glm::mat4& value) const {
	assert(m_is_generated);
	bind();
	
	int uniform_location = glGetUniformLocation(m_id, name.c_str());
	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value));
}
void RenderProgram::set_uniforms(const std::string& view_transformation_uniform_name, const std::string& projection_transformation_uniform_name, const std::string& camera_eye_position_uniform_name, const std::string& camera_up_direction_uniform_name, const Camera& camera) const {
	set_uniform(view_transformation_uniform_name, camera.get_view_transformation());
	set_uniform(projection_transformation_uniform_name, camera.get_projection_transformation());
	set_uniform(camera_eye_position_uniform_name, camera.get_eye_position());
	set_uniform(camera_up_direction_uniform_name, camera.get_up_direction());
}
void RenderProgram::destroy() {
	if (m_is_generated) {
		glDeleteProgram(m_id);
		m_is_generated = false;
	}
}
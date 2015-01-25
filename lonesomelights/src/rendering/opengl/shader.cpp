#include "rendering/opengl/shader.h"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <utility>

Shader::Shader(GLenum shader_type)
	: m_shader_type(shader_type),
	m_is_generated(false),
	m_id() {
}
Shader::Shader(Shader&& shader)
	: m_shader_type(std::move(shader.m_shader_type)),
	m_is_generated(std::move(shader.m_is_generated)),
	m_id(std::move(shader.m_id)) {
	shader.m_is_generated = false;
}
Shader& Shader::operator=(Shader&& shader) {
	std::swap(m_shader_type, shader.m_shader_type);
	std::swap(m_is_generated, shader.m_is_generated);
	std::swap(m_id, shader.m_id);
	return *this;
}
Shader::~Shader() {
	destroy();
}

GLuint Shader::get_id() const {
	assert(m_is_generated);
	return m_id;
}

bool Shader::load_from_file(const std::string& file_path) {
	destroy();

	// Read shader source from file.

	std::string file_content;
    std::ifstream file_stream(file_path, std::ios::in);
    if(!file_stream.is_open()) {
        std::cerr << "Rendering: Could not read file '" << file_path << "'." << std::endl;
		
		m_is_generated = false;
		return false;
    }
    std::string file_line = "";
    while(!file_stream.eof()) {
        std::getline(file_stream, file_line);
        file_content.append(file_line + "\n");
    }
    file_stream.close();
	const char* shader_source = file_content.c_str();

	// Create and compile shader.
	
	m_id = glCreateShader(m_shader_type);
	glShaderSource(m_id, 1, &shader_source, NULL);
	glCompileShader(m_id);

	GLint result = GL_FALSE;
	glGetShaderiv(m_id, GL_COMPILE_STATUS, &result);
	if (!result) {
		GLint log_length;
		glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &log_length);
		GLchar* log = new GLchar[log_length];
		glGetShaderInfoLog(m_id, log_length, &log_length, log);
		std::cerr << "Rendering: Failed to compile shader from file '" << file_path << "':" << std::endl << log << std::endl;
		delete log;
		
		m_is_generated = false;
		return false;
	}
	
	m_is_generated = true;
	return true;
}
void Shader::destroy() {
	if (m_is_generated) {
		glDeleteShader(m_id);
		m_is_generated = false;
	}
}

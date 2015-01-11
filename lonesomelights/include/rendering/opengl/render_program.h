#ifndef __RENDERING__OPENGL__RENDER_PROGRAM_H__
#define __RENDERING__OPENGL__RENDER_PROGRAM_H__

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class FragmentShader;
class VertexShader;

class RenderProgram {
public:
	static void unbind_any();

	RenderProgram();
	RenderProgram(const RenderProgram& render_program) = delete;
	RenderProgram(RenderProgram&& render_program);
	RenderProgram& operator=(const RenderProgram& render_program) = delete;
	RenderProgram& operator=(RenderProgram&& render_program);
	~RenderProgram();
	
	bool link(const VertexShader& vertex_shader, const FragmentShader& fragment_shader);
	
	void bind() const;
	void set_uniform(const std::string& name, float value) const;
	void set_uniform(const std::string& name, const glm::vec3& value) const;
	void destroy();
private:
	bool m_is_generated;
	GLuint m_id;
};

#endif
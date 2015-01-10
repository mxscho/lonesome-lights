#ifndef __RENDERING__OPENGL__SHADER_H__
#define __RENDERING__OPENGL__SHADER_H__

#include <string>
#include <GL/glew.h>

class Shader {
public:
	explicit Shader(GLenum shader_type);
	Shader(const Shader& shader) = delete;
	Shader(Shader&& shader);
	Shader& operator=(const Shader& shader) = delete;
	Shader& operator=(Shader&& shader);
	~Shader();
	
	GLuint get_id() const;
	
	bool load_from_file(const std::string& file_path);
	void destroy();
private:
	GLenum m_shader_type;
	bool m_is_generated;
	GLuint m_id;
};

#endif
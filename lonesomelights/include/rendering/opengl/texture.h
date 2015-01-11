#ifndef __RENDERING__OPENGL__TEXTURE_H__
#define __RENDERING__OPENGL__TEXTURE_H__

#include <string>
#include <GL/glew.h>

class Texture {
public:
	static void unbind_any(GLenum texture);

	Texture();
	Texture(const Texture& texture) = delete;
	Texture(Texture&& texture);
	Texture& operator=(const Texture& texture) = delete;
	Texture& operator=(Texture&& texture);
	~Texture();
	
	GLuint get_id() const;
	
	bool load_from_file(const std::string& file_path, GLenum texture);
	
	void bind(GLenum texture) const;
	void destroy();
private:
	GLuint m_id;
	bool m_is_generated;
	unsigned int m_width;
	unsigned int m_height;
};

#endif
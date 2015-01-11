#ifndef __RENDERING__OPENGL__VERTEX_ARRAY_OBJECT_H__
#define __RENDERING__OPENGL__VERTEX_ARRAY_OBJECT_H__

#include <GL/glew.h>
#include <vector>

// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/

class FrameBufferObject {
public:
	static void unbind_any();

	FrameBufferObject(unsigned int width, unsigned int height);
	FrameBufferObject(const FrameBufferObject& vertex_array_object) = delete;
	FrameBufferObject(FrameBufferObject&& vertex_array_object);
	FrameBufferObject& operator=(const FrameBufferObject& vertex_array_object) = delete;
	FrameBufferObject& operator=(FrameBufferObject&& vertex_array_object);
	~FrameBufferObject();
	
	GLuint get_id() const;
	bool check_status() const;
	void attach_depthbuffer_from_texture(GLuint texture);
	void attach_colorbuffer_from_texture(GLuint texture);
	void setDrawbuffers();

	
	void bind();
private:
	void attach_texture(GLenum attachment, GLuint texture);

	unsigned int m_width;
	unsigned int m_height;
	bool m_is_generated;
	GLuint m_id;
	unsigned int m_number_of_colorbuffers;
	std::vector<GLenum> m_drawBuffers;
	bool m_drawbuffers_set;
};

#endif
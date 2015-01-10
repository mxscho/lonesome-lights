#ifndef __RENDERING__OPENGL__VERTEX_ARRAY_OBJECT_H__
#define __RENDERING__OPENGL__VERTEX_ARRAY_OBJECT_H__

#include <GL/glew.h>

class VertexArrayObject {
public:
	static void unbind_any();

	VertexArrayObject();
	VertexArrayObject(const VertexArrayObject& vertex_array_object) = delete;
	VertexArrayObject(VertexArrayObject&& vertex_array_object);
	VertexArrayObject& operator=(const VertexArrayObject& vertex_array_object) = delete;
	VertexArrayObject& operator=(VertexArrayObject&& vertex_array_object);
	~VertexArrayObject();
	
	GLuint get_id() const;
	
	void bind() const;
private:
	bool m_is_generated;
	GLuint m_id;
};

#endif
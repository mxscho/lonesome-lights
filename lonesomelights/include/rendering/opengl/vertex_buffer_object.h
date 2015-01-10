#ifndef __RENDERING__OPENGL__VERTEX_BUFFER_OBJECT_H__
#define __RENDERING__OPENGL__VERTEX_BUFFER_OBJECT_H__

#include <functional>
#include <list>
#include <vector>
#include <GL/glew.h>

template<typename T>
class VertexBufferObject {
public:
	class Bucket {
	public:
		Bucket(VertexBufferObject<T>& vertex_buffer_object, unsigned int index);
		Bucket(const Bucket& bucket) = delete;
		Bucket(Bucket&& bucket);
		Bucket& operator=(const Bucket& bucket) = delete;
		Bucket& operator=(Bucket&& bucket);
		~Bucket();

		const T& get() const;
		void set(const T& value);
	private:
		VertexBufferObject<T>& m_vertex_buffer_object;
		unsigned int m_index;
		bool m_in_use;
	};

	VertexBufferObject(const std::vector<T>& data, GLenum usage);
	VertexBufferObject(unsigned int size, GLenum usage);
	VertexBufferObject(const VertexBufferObject& vertex_buffer_object) = delete;
	VertexBufferObject(VertexBufferObject&& vertex_buffer_object);
	VertexBufferObject& operator=(const VertexBufferObject& vertex_buffer_object) = delete;
	VertexBufferObject& operator=(VertexBufferObject&& vertex_buffer_object);
	~VertexBufferObject();

	Bucket claim_bucket();
	
	void bind() const;
	void update();
	void destroy();
private:
	VertexBufferObject(unsigned int size, GLenum target, GLenum usage, const std::vector<T>& data);

	GLuint m_id;
	bool m_is_generated;
	GLenum m_target;
	GLenum m_usage;
	unsigned int m_size;
	std::vector<T> m_data;
	unsigned int m_changed_begin_index;
	unsigned int m_changed_end_index;
	std::list<unsigned int> m_unused_indices;
};

#include "rendering/opengl/vertex_buffer_object.tpp"

#endif
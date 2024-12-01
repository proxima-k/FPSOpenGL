#include "VertexBuffer.h"

#include "Renderer.h"

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size, bool isDataStatic)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

	if (isDataStatic) {
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		return;
	}

	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

// only for updating existing data with the same layout
// WARNING: this doesn't support changing the data layouts
// bool parameter means is the data being updated going to have more/less data or the same amount in terms of size
// this is to prevent GPU from reallocating unneccesary new memory to store data and just update the existing memory
void VertexBuffer::updateData(const void* data, unsigned int size, bool isDataSizeSameAsPrevious)
{
	// be aware of active VAOs. To prevent affecting them, unbind the active VAO
	this->Bind();

	if (isDataSizeSameAsPrevious)
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	else
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size, bool isDataStatic = true);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void updateData(const void* data, unsigned int size, bool isDataSizeSameAsPrevious = true);
};
#include "pch.h"
#include "VertexBuffer.h"

namespace yoi
{
	VertexBuffer::VertexBuffer()
		:m_Offset(0), m_VertexCount(0)
	{}
	VertexBuffer::VertexBuffer(const Buffer& buffer, size_t offset, unsigned int vertexCount)
		:Buffer(buffer), m_Offset(offset), m_VertexCount(vertexCount)
	{
	}
	void VertexBuffer::SetVertexCount(unsigned int vertexCount)
	{
		m_VertexCount = vertexCount;
	}
	unsigned int VertexBuffer::GetVertexCount() const
	{
		return m_VertexCount;
	}
	void VertexBuffer::SetOffset(size_t offset)
	{
		m_Offset = offset;
	}
	size_t VertexBuffer::GetOffset() const
	{
		return m_Offset;
	}
	void VertexBuffer::SetLayoutType(int type)
	{
		m_VertexBufferLayout.typeLayout = type;
	}
}
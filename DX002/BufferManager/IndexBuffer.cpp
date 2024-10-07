#include "pch.h"
#include "IndexBuffer.h"

namespace yoi
{
	IndexBuffer::IndexBuffer()
		:m_IndexCount(0), m_Offset(0)
	{
	}
	IndexBuffer::IndexBuffer(const Buffer& buffer, unsigned int indexCount, unsigned int offset)
		:Buffer(buffer), m_IndexCount(indexCount), m_Offset(offset)
	{
	}
	IndexBuffer::IndexBuffer(Buffer&& buffer, unsigned int indexCount, unsigned int offset)
		:Buffer(std::move(buffer)), m_IndexCount(indexCount), m_Offset(offset)
	{
	}
	unsigned int IndexBuffer::GetIndexCount() const
	{
		return m_IndexCount;
	}
	unsigned int IndexBuffer::GetOffset() const
	{
		return m_Offset;
	}
	void IndexBuffer::SetIndexCount(unsigned int count)
	{
		m_IndexCount = count;
	}
	void IndexBuffer::SetOffset(unsigned int offset)
	{
		m_Offset = offset;
	}
}
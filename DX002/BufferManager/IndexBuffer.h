#pragma once
#include "Buffer.h"

namespace yoi
{
	class IndexBuffer : public Buffer
	{
	protected:
		unsigned int m_IndexCount;
		unsigned int m_Offset;
	public:
		IndexBuffer();
		IndexBuffer(const Buffer& buffer, unsigned int indexCount, unsigned int offset);
		IndexBuffer(Buffer&& buffer, unsigned int indexCount, unsigned int offset);
		~IndexBuffer() = default;
		IndexBuffer& operator = (const IndexBuffer&) = delete;
		IndexBuffer(const IndexBuffer&) = delete;
		
		unsigned int GetIndexCount() const;
		unsigned int GetOffset() const;

		void SetIndexCount(unsigned int count);
		void SetOffset(unsigned int offset);

	};
}
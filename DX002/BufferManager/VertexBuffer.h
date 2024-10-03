#pragma once
#include "Buffer.h"

namespace yoi
{
	class VertexBuffer : public Buffer
	{
	public:
		struct Layout
		{
			// size of type and dimension of every attribution
			using Distribution = std::pair<int, int>;
			std::vector<Distribution> distributions;
			int typeLayout;	// 0: 按顶点排列。每一个顶点的属性紧挨着。1: 按属性排列。相同的属性紧挨着。

			Layout() :typeLayout(0) {}
		};
	protected:
		Layout m_VertexBufferLayout;
		size_t m_Offset;
		unsigned int m_VertexCount;
		
	public:
		VertexBuffer();
		explicit VertexBuffer(const Buffer& buffer, size_t offset, unsigned int vertexCount);
		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator = (const VertexBuffer&) = delete;
		~VertexBuffer() = default;

		void SetVertexCount(unsigned int vertexCount);
		unsigned int GetVertexCount() const;

		void SetOffset(size_t offset);
		size_t GetOffset() const;
		// type: 0: 按顶点排列。每一个顶点的属性紧挨着。1: 按属性排列。相同的属性紧挨着。
		void SetLayoutType(int type);

		template<typename T>
		void PushAttrib(int dimension)
		{
			m_VertexBufferLayout.distributions.emplace_back(sizeof(T), dimension);
		}

		

	};
}
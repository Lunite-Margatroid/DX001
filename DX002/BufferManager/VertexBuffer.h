#pragma once
#include "Buffer.h"

namespace yoi
{
	class VertexBuffer : public Buffer
	{
	public:
		template<typename T, int d>
		struct Distrib
		{
			using type = T;
			static int dimension() { return d; };
		};


		struct Layout
		{
			// size of type and dimension of every attribution
			using Distribution = std::pair<int, int>;
			std::vector<Distribution> distributions;
			int typeLayout;	// 0: 按顶点排列。每一个顶点的属性紧挨着。1: 按属性排列。相同的属性紧挨着。

			Layout() :typeLayout(0) {}
			Layout(Layout&& layout) noexcept : distributions(std::move(layout.distributions)), typeLayout(layout.typeLayout)  {}
		};
	protected:
		
		size_t m_Offset;
		unsigned int m_VertexCount;
	public:
		Layout m_VertexBufferLayout;
		
	public:
		VertexBuffer();
		VertexBuffer(const Buffer& buffer, size_t offset, unsigned int vertexCount);
		VertexBuffer(Buffer&& buffer, size_t offset, unsigned int vertexCount);
		VertexBuffer(VertexBuffer&& other) noexcept;
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
		
		template<typename ... Args>
		void SetLayout()
		{
			// (PushAttrib<Args::type>(Args::dimension()), ...);

			(m_VertexBufferLayout.distributions.emplace_back(sizeof(Args::type), Args::dimension()), ...);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pContext"></param>
		/// <param name="slot"></param>
		/// <returns> the num of slots this vertexbuffer occupy </returns>
		unsigned int Bind(ID3D11DeviceContext* pContext, unsigned int slot);
	};
}
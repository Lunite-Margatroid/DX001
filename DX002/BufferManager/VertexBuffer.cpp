#include "pch.h"
#include "VertexBuffer.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"
#include "Logger\FileLogger.h"

namespace yoi
{
	VertexBuffer::VertexBuffer()
		:m_Offset(0), m_VertexCount(0)
	{}
	VertexBuffer::VertexBuffer(const Buffer& buffer, size_t offset, unsigned int vertexCount)
		:Buffer(buffer), m_Offset(offset), m_VertexCount(vertexCount)
	{
	}
	VertexBuffer::VertexBuffer(Buffer&& buffer, size_t offset, unsigned int vertexCount)
		:Buffer(std::move(buffer)), m_Offset(offset), m_VertexCount(vertexCount)
	{
	}
	VertexBuffer::VertexBuffer(ID3D11Device* pDevice, void* data, size_t size)
		:m_Offset(0),m_VertexCount(0)
	{
		GFX_EXCEPT_SUPPORT();
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = size;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = data;

		GFX_THROW_INFO(pDevice->CreateBuffer(&bufferDesc, &subData, &m_d3dBuffer));
		
	}
	VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
		:m_Offset(other.m_Offset),
		m_VertexCount(other.m_VertexCount),
		m_VertexBufferLayout(std::move(other.m_VertexBufferLayout))
	{
		m_d3dBuffer = std::move(other.m_d3dBuffer);
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
	unsigned int VertexBuffer::Bind(ID3D11DeviceContext* pContext, unsigned int slot)
	{
		GFX_EXCEPT_SUPPORT();
		unsigned int numSlots = 0;

		if (m_VertexBufferLayout.distributions.size() == 0)
		{
			FileLogger::Warn("VertexBuffer::Bind(). The layout didn't set.");
		}

		if (m_VertexBufferLayout.typeLayout == 0)
		{
			unsigned int stride = 0u;
			unsigned int offset = m_Offset;
			for (auto [size, dimension] : m_VertexBufferLayout.distributions)
			{
				stride += size * dimension;
			}
			GFX_THROW_INFO_ONLY(pContext->IASetVertexBuffers(slot, 1,m_d3dBuffer.GetAddressOf(), &stride, &offset));
			numSlots += 1;
		}
		else if (m_VertexBufferLayout.typeLayout == 1)
		{
			unsigned int offset = m_Offset;
			for (auto [size, dimension] : m_VertexBufferLayout.distributions)
			{
				unsigned int stripe = size * dimension;
				GFX_THROW_INFO_ONLY(pContext->IASetVertexBuffers(slot++, 1, m_d3dBuffer.GetAddressOf(), &stripe, &offset));
				offset += stripe * m_VertexCount;
				numSlots += 1;
			}
		}
		else
		{
			FileLogger::Warn("VerterBuffer::Bind(). Layout type is wrong.");
		}

		return numSlots;
	}
	void VertexBuffer::UnbindVertexBuffer(ID3D11DeviceContext* pContext, unsigned int slot)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11Buffer* null_ptr = nullptr;
		unsigned int temp = 0u;
		GFX_THROW_INFO_ONLY(pContext->IASetVertexBuffers(slot, 1, &null_ptr, &temp, &temp));
	}
}
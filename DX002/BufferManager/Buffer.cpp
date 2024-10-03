#include "pch.h"
#include "Buffer.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	Buffer::Buffer()
		:m_d3dBuffer(nullptr)
	{
	}
	Buffer::Buffer(const Buffer& other)
	{
		m_d3dBuffer = other.m_d3dBuffer;
	}
	Buffer::Buffer(Buffer&& other) noexcept
	{
		m_d3dBuffer = other.m_d3dBuffer;
		other.m_d3dBuffer.Reset();
		other.m_d3dBuffer = nullptr;
	}
	Buffer& Buffer::operator=(const Buffer& other)
	{
		m_d3dBuffer = other.m_d3dBuffer;
		return*this;
	}
	Buffer& Buffer::operator=(Buffer&& other) noexcept
	{
		m_d3dBuffer = other.m_d3dBuffer;
		other.m_d3dBuffer.Reset();
		other.m_d3dBuffer = nullptr;
		return*this;
	}
	void Buffer::Init(ID3D11Device* pDevice, const D3D11_BUFFER_DESC* bd, const D3D11_SUBRESOURCE_DATA* sd)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO(pDevice->CreateBuffer(bd, sd, &m_d3dBuffer));
	}
	ID3D11Buffer* Buffer::GetBuffer()
	{
		return m_d3dBuffer.Get();
	}
}

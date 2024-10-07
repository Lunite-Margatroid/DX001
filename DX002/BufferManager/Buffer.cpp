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
	Buffer::Buffer(ID3D11Device* pDevice, const D3D11_BUFFER_DESC* bd, const D3D11_SUBRESOURCE_DATA* sd)
	{
		Init(pDevice, bd, sd);
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
	void Buffer::SetBufferData(ID3D11DeviceContext* pContext, void* src, size_t size, size_t offset)
	{
		GFX_EXCEPT_SUPPORT();

		D3D11_MAPPED_SUBRESOURCE dataMap;
		GFX_THROW_INFO(pContext->Map(m_d3dBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &dataMap));
		memcpy(((BYTE*)dataMap.pData) + offset, src, size);
		GFX_THROW_INFO_ONLY(pContext->Unmap(m_d3dBuffer.Get(), 0));
	}
}

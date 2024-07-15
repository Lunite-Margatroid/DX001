#include "pch.h"
#include "BufferManager.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	BufferManager::BufferManager(ID3D11Device* pDevice)
		:m_pDevice(pDevice)
	{
	}
	BufferManager::~BufferManager()
	{
		for (ID3D11Buffer* pBuffer : m_Buffers)
		{
			pBuffer->Release();
		}
	}
	ID3D11Buffer* BufferManager::AddBuffer(const D3D11_BUFFER_DESC* pDescript, const D3D11_SUBRESOURCE_DATA* initData)
	{
		GFX_EXCEPT_SUPPORT();
		m_Buffers.push_back(nullptr);
		GFX_THROW_INFO(m_pDevice->CreateBuffer(pDescript, initData, &m_Buffers.back()));
		return m_Buffers.back();
	}
}
#include "pch.h"
#include "ConstBuffer.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{



	ConstBuffer::ConstBuffer(ID3D11Device* pDevice, size_t size, const void* data)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = size;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.StructureByteStride = 0u;
		bd.MiscFlags = 0u;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = data;
		sd.SysMemPitch = 0u;

		if (data == nullptr)
		{
			std::unique_ptr<BYTE[]> t(new BYTE[size]);
			memset(t.get(), 0, size);
			sd.pSysMem = t.get();
			Init(pDevice, &bd, &sd);
		}
		else
			Init(pDevice, &bd, &sd);
	}

	ConstBuffer::ConstBuffer(ID3D11Device* pDevice, size_t size, const D3D11_SUBRESOURCE_DATA* sd)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = size;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.StructureByteStride = 0u;
		bd.MiscFlags = 0u;

		Init(pDevice, &bd, sd);
	}

	void ConstBuffer::Bind(ID3D11DeviceContext* pContext, unsigned int slot)
	{
		BindVS(pContext, 0u);
	}

	void ConstBuffer::BindVS(ID3D11DeviceContext* pContext, unsigned int slot)
	{
		GFX_EXCEPT_SUPPORT();
		pContext->VSSetConstantBuffers(slot, 1, m_d3dBuffer.GetAddressOf());
	}

	void ConstBuffer::BindGS(ID3D11DeviceContext* pContext, unsigned int slot)
	{
		GFX_EXCEPT_SUPPORT();
		pContext->GSSetConstantBuffers(slot, 1, m_d3dBuffer.GetAddressOf());
	}

	void ConstBuffer::BindCS(ID3D11DeviceContext* pContext, unsigned int slot)
	{
		GFX_EXCEPT_SUPPORT();
		pContext->CSSetConstantBuffers(slot, 1, m_d3dBuffer.GetAddressOf());
	}

	void ConstBuffer::BindPS(ID3D11DeviceContext* pContext, unsigned int slot)
	{
		GFX_EXCEPT_SUPPORT();
		pContext->PSSetConstantBuffers(slot, 1, m_d3dBuffer.GetAddressOf());
	}

}

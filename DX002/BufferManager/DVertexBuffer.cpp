#include "pch.h"
#include "DVertexBuffer.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	yoi::DVertexBuffer::DVertexBuffer( VertexBuffer&& vertexBuffer, unsigned int height, unsigned int width, unsigned int dimension)
		:VertexBuffer(std::move(vertexBuffer)),
		m_Height(height),
		m_Width(width),
		m_Dimension(dimension)
	{
		DXGI_FORMAT format;
		switch (m_Dimension)
		{
		case 1: format = DXGI_FORMAT_R32_FLOAT;				break;
		case 2: format = DXGI_FORMAT_R32G32B32_FLOAT;		break;
		case 3: format = DXGI_FORMAT_R32G32B32_FLOAT;		break;
		case 4: format = DXGI_FORMAT_R32G32B32A32_FLOAT;	break;
		default:format = DXGI_FORMAT_R32G32B32_FLOAT;		break;
		}

		assert(m_Dimension == 1 || m_Dimension == 4);

		ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavd = {};
		uavd.Format = format;
		uavd.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavd.Buffer.NumElements = m_Height * m_Width;
		uavd.Buffer.FirstElement = 0u;
		uavd.Buffer.Flags = 0u;

		pDevice->CreateUnorderedAccessView(m_d3dBuffer.Get(), &uavd, &m_uav);
	}
	void DVertexBuffer::BindCS(ID3D11DeviceContext* pContext, unsigned int slot)
	{
		GFX_EXCEPT_SUPPORT();
		GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(slot, 1, m_uav.GetAddressOf(), nullptr));
	}
	void DVertexBuffer::UnbindCS(ID3D11DeviceContext* pContext, unsigned int slot)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11UnorderedAccessView* null_ptr = nullptr;
		GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(slot, 1, &null_ptr, nullptr));
	}
}
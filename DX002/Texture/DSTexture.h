#pragma once
#include "BasicTexture2D.h"

namespace yoi
{
	class DSTexture : public BasicTexture2D
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSView;
	public:
		DSTexture(ID3D11Device* pDevice, unsigned int width, unsigned int height);
		~DSTexture() = default;

		ID3D11DepthStencilView* GetDSView();

		/********* defulat bind func ****************/

		virtual void Bind(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void Bind(UINT slot = 0u) override;
		virtual void Unbind(UINT slot = 0u) override;
		virtual void Unbind(ID3D11DeviceContext* pContext, UINT slot = 0u) override;

		/******************************************/

		virtual void BindCS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void BindPS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void BindGS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void BindVS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;

		virtual void UnbindCS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void UnbindPS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void UnbindGS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void UnbindVS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
	};
}
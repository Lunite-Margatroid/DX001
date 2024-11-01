/*
* texture for off-screen render.
* build 2 views on the texture.
* Shader resource view and render target view.
*/
#pragma once

#include "Texture.h"

namespace yoi
{
	class OSRTexture : public Texture
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

	public:
		OSRTexture(ID3D11Device* pDevice, unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);

		void BindRenderTarget(ID3D11DeviceContext* pContext);
		ID3D11RenderTargetView* GetRenderTarget();
	};
};
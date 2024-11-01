/*
* Pipeline to render off-screen texture on screen.
* Expected to call methord below before render.
* void AddRenderTaget(ID3D11RenderTargetView* renderTarget);
* void SetDepthStencilView(ID3D11DepthStencilView* depthStencilView);
* void SetViewPort(const D3D11_VIEWPORT* viewPort);
*/

#pragma once
#include "Pipeline.h"
#include "Shader\Shader.h"
#include "BufferManager\VertexBuffer.h"
namespace yoi
{
	class Screen : public Pipeline
	{
	protected:
		void Init();

	public:
		Screen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	
		~Screen();
	};
}
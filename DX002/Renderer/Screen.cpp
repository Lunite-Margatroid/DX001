#include "pch.h"
#include "Screen.h"
#include "Graphics.h"
#include "GFXMacro.h"

namespace yoi
{
	void Screen::Init()
	{
		GFX_EXCEPT_SUPPORT();

		/*********** Init rasterizer state***************/
		D3D11_RASTERIZER_DESC stt = {};
		stt.FillMode = D3D11_FILL_SOLID;
		stt.CullMode = D3D11_CULL_NONE;
		stt.FrontCounterClockwise = false;
		stt.DepthBias = 0;
		stt.DepthBiasClamp = 0.0f;
		stt.ScissorEnable = false;
		stt.DepthClipEnable = false;
		stt.ScissorEnable = false;
		stt.MultisampleEnable = false;
		stt.AntialiasedLineEnable = false;

		GFX_THROW_INFO(m_pDevice->CreateRasterizerState(&stt, &m_pRasterizerState));

		/************* Init depthstencil test state ******************/

		D3D11_DEPTH_STENCIL_DESC dsd = {};
		dsd.DepthEnable = false;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;
		dsd.StencilEnable = false;
		dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

		GFX_THROW_INFO(m_pDevice->CreateDepthStencilState(&dsd, &m_DepthStencilState));

		/**************** init blend state ********************/
		D3D11_BLEND_DESC bd = {};
		bd.AlphaToCoverageEnable = false;
		bd.IndependentBlendEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = static_cast<unsigned char>(0xfu);

		GFX_THROW_INFO(m_pDevice->CreateBlendState(&bd, &m_pBlendState));

	}
	Screen::Screen(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		:Pipeline(pDevice, pContext)
	{
		Init();
	}
	Screen::~Screen()
	{
		if (m_pRasterizerState)
			delete m_pRasterizerState;
		if (m_DepthStencilState)
			delete m_DepthStencilState;
		if (m_pBlendState)
			delete m_pBlendState;
	}




}
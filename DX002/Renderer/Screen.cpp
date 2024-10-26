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

		/************ Init vertex buffer ************/
		struct Vertex
		{
			struct
			{
				float x, y;
			}Postion;
			struct
			{
				float u, v;
			}TexCoord;
		};

		Vertex vertexData[] =
		{
			{{-1.0f, -1.0f}, {0.0f, 1.0f}},
			{{1.0f, -1.0f}, {1.0f, 1.0f}},
			{{-1.0f, 1.0f}, {0.0f, 0.0f}},
			{{1.0f, 1.0f}, {1.0f, 0.0f}}
		};

		m_pVertexBuffer = std::make_unique<VertexBuffer>(m_pDevice, vertexData, sizeof(vertexData));
		m_pVertexBuffer->SetVertexCount(4);
		m_pVertexBuffer->SetLayout<VertexBuffer::Distrib<float, 2>, VertexBuffer::Distrib<float, 2>>();


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

	void Screen::Flush()
	{
		GFX_EXCEPT_SUPPORT();

		GFX_THROW_INFO_ONLY(m_pContext->OMSetDepthStencilState(m_DepthStencilState, m_StencilRef));
		GFX_THROW_INFO_ONLY(m_pContext->OMSetBlendState(m_pBlendState, m_BlendFactor, m_SampleMask));
		GFX_THROW_INFO_ONLY(m_pContext->RSSetViewports(1, &m_ViewPort));
		GFX_THROW_INFO_ONLY(m_pContext->OMSetRenderTargets(m_RenderTargets.size(), m_RenderTargets.data(), m_DepthStencilVeiw));
		GFX_THROW_INFO_ONLY(m_pContext->RSSetState(m_pRasterizerState));

		// clear buffer
		GFX_THROW_INFO_ONLY(m_pContext->ClearRenderTargetView(m_RenderTargets[0], m_ClearColor));
		GFX_THROW_INFO_ONLY(m_pContext->ClearDepthStencilView(m_DepthStencilVeiw, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0x00));

		m_pVertexBuffer->Bind(m_pContext, 0);
		m_Shader->Bind(m_pContext);

		GFX_THROW_INFO_ONLY(m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP));

		GFX_THROW_INFO_ONLY(m_pContext->Draw(m_pVertexBuffer->GetVertexCount(), 0u));
	}

	void Screen::Render(CameraObj* camera, SceneObj* scene)
	{
		assert(false);
	}
	void Screen::SetShader(Shader* shader)
	{
		m_Shader = shader;
	}
}
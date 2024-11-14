#include "pch.h"
#include "PipelineV1.h"
#include "Renderer/Graphics.h"
#include "Renderer/GFXMacro.h"

namespace yoi
{
	void PipelineV1::PushRenderQueue(SceneObj* scene, const glm::mat4& modelTrans)
	{
		if (!scene->m_Visible) return;
		scene->UpdateModelTrans(modelTrans);
		SpriteV3* sprite = reinterpret_cast<SpriteV3*>(scene->m_Sprite);
		if (sprite)
		{
			switch (sprite->m_Type)
			{
			case Sprite::SpriteType::Untransparent:
				m_RenderQueueMain.push(RenderItem(scene->m_ModelTrans, sprite));
				break;
			case Sprite::SpriteType::Skybox:
				m_RenderQueueSkyb.push(RenderItem(scene->m_ModelTrans, sprite));
			};
			
		}
		for (SceneObj* child : scene->m_ChildObj)
		{
			PushRenderQueue(child, scene->m_ModelTrans);
		}
	}

	PipelineV1::PipelineV1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		:Pipeline(pDevice, pContext)
	{
		/* 1 ******* Create DS state for main rendering *********/
		D3D11_DEPTH_STENCIL_DESC dsd = {};
		dsd.DepthEnable = true;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS;

		dsd.StencilEnable = false;
		dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

		dsd.BackFace = dsd.FrontFace;

		pDevice->CreateDepthStencilState(&dsd, &m_DepthStencilState);

		/* 2 ******** Create Rasterizer state for main rendering **********/
		D3D11_RASTERIZER_DESC rd = {};
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		rd.FrontCounterClockwise = false;

		rd.DepthBias = 0;
		rd.DepthBiasClamp = 0.0f;
		rd.SlopeScaledDepthBias = 0.0f;

		rd.DepthClipEnable = true;

		rd.ScissorEnable = false;

		rd.MultisampleEnable = false;
		rd.AntialiasedLineEnable = false;
		pDevice->CreateRasterizerState(&rd, &m_pRasterizerState);

		/* 3 ****** Create blend state for main rendering ********/
		D3D11_BLEND_DESC bd = {};
		bd.AlphaToCoverageEnable = false;
		bd.IndependentBlendEnable = false;

		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = 0xffffffff;

		pDevice->CreateBlendState(&bd, &m_pBlendState);

		/* 4 ***** Create DS state for skybox rendering *****/
		dsd.DepthEnable = true;
		dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		dsd.StencilEnable = false;
		dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

		dsd.BackFace = dsd.FrontFace;

		pDevice->CreateDepthStencilState(&dsd, &m_pDSStateSkybox);


		/* 5 ******** Create Rasterizer state for skybox rendering **********/
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_FRONT;
		rd.FrontCounterClockwise = false;

		rd.DepthBias = 0;
		rd.DepthBiasClamp = 0.0f;
		rd.SlopeScaledDepthBias = 0.0f;

		rd.DepthClipEnable = true;

		rd.ScissorEnable = false;

		rd.MultisampleEnable = false;
		rd.AntialiasedLineEnable = false;
		pDevice->CreateRasterizerState(&rd, &m_RasterizerStateSkyb);
	}

	PipelineV1::~PipelineV1()
	{
		// 1
		m_DepthStencilState->Release();
		// 2
		m_pRasterizerState->Release();
		// 3
		m_pBlendState->Release();
		// 4
		m_pDSStateSkybox->Release();
		// 5
		m_RasterizerStateSkyb->Release();

	}

	void PipelineV1::Render(CameraObj* camera, SceneObj* scene)
	{
		GFX_EXCEPT_SUPPORT();
		// create render queue
		glm::mat4 eye(1.0f);
		PushRenderQueue(scene, eye);

		// render normal object
		GFX_THROW_INFO_ONLY(m_pContext->OMSetDepthStencilState(m_DepthStencilState, m_StencilRef));
		GFX_THROW_INFO_ONLY(m_pContext->OMSetBlendState(m_pBlendState, m_BlendFactor, m_SampleMask));
		GFX_THROW_INFO_ONLY(m_pContext->RSSetViewports(1, &m_ViewPort));
		GFX_THROW_INFO_ONLY(m_pContext->OMSetRenderTargets(m_RenderTargets.size(), m_RenderTargets.data(), m_DepthStencilVeiw));
		GFX_THROW_INFO_ONLY(m_pContext->RSSetState(m_pRasterizerState));
		GFX_THROW_INFO_ONLY(m_pContext->ClearRenderTargetView(m_RenderTargets[0], m_ClearColor));
		GFX_THROW_INFO_ONLY(m_pContext->ClearDepthStencilView(m_DepthStencilVeiw, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0x00));

		while (!m_RenderQueueMain.empty())
		{
			m_RenderQueueMain.front()(camera);
			m_RenderQueueMain.pop();
		}

		// render skybox
		GFX_THROW_INFO_ONLY(m_pContext->OMSetDepthStencilState(m_pDSStateSkybox, m_StencilRef));
		while (!m_RenderQueueSkyb.empty())
		{
			m_RenderQueueSkyb.front()(camera);
			m_RenderQueueSkyb.pop();
		}

	}
}
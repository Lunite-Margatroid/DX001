#include "pch.h"
#include "Pipeline.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
    void Pipeline::PushRenderQueue(SceneObj* scene, const glm::mat4& modelTrans)
    {
        if (!scene->m_Visible) return;
        scene->UpdateModelTrans(modelTrans);
        if(scene->m_Sprite)
            m_RenderQueueMain.push(RenderItem(scene->m_ModelTrans, static_cast<SpriteV3*>(scene->m_Sprite)));
        for (SceneObj* child : scene->m_ChildObj)
        {
            PushRenderQueue(child, scene->m_ModelTrans);
        }
    }
    Pipeline::Pipeline(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
        :
        m_DepthStencilVeiw(nullptr),
        m_DepthStencilState(nullptr),
        m_ViewPort({}),
        m_pRasterizerState(nullptr),
        m_pBlendState(nullptr),
        m_StencilRef(0u), m_SampleMask(0xffffffff),
        m_pDevice(pDevice), m_pContext(pContext)
    {
        m_BlendFactor[0] = 0.0f;
        m_BlendFactor[1] = 0.0f;
        m_BlendFactor[2] = 0.0f;
        m_BlendFactor[3] = 0.0f;

        m_ClearColor[0] = 0.0f;
        m_ClearColor[1] = 0.0f;
        m_ClearColor[2] = 0.0f;
        m_ClearColor[3] = 1.0f;
    }
    void Pipeline::AddRenderTaget(ID3D11RenderTargetView* renderTarget)
    {
        m_RenderTargets.push_back(renderTarget);
    }
    void Pipeline::SetDepthStencilView(ID3D11DepthStencilView* depthStencilView)
    {
        m_DepthStencilVeiw = depthStencilView;
    }
    void Pipeline::ClearRenderTarget()
    {
        m_RenderTargets.clear();
    }
    void Pipeline::SetRasterizerState(ID3D11RasterizerState* rasterizerState)
    {
        m_pRasterizerState = rasterizerState;
    }
    void Pipeline::SetDepthStencilState(ID3D11DepthStencilState* depthStencilState)
    {
        m_DepthStencilState = depthStencilState;
    }
    void Pipeline::SetViewPort(const D3D11_VIEWPORT* viewPort)
    {
        m_ViewPort = *viewPort;
    }
    void Pipeline::Render(CameraObj* camera, SceneObj* scene)
    {
        GFX_EXCEPT_SUPPORT();
        // create render queue
        glm::mat4 eye(1.0f);
        PushRenderQueue(scene, eye);

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

    }
}
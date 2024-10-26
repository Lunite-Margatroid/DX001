#pragma once
#include <d3d11.h>
#include "SceneObject\CameraObj.h"

namespace yoi
{
	class Pipeline
	{
	protected:
		class RenderItem
		{
		private:
			const glm::mat4& m_ModelMat;
			SpriteV3* m_Sprite;
		public:
			RenderItem(const glm::mat4& modelMat, SpriteV3* sprite)
				:m_ModelMat(modelMat), m_Sprite(sprite)
			{}

			inline void operator() (CameraObj* camera)
			{
				m_Sprite->RenderV3(m_ModelMat, camera);
			}
		};

	protected:
		// render target views
		std::vector<ID3D11RenderTargetView*> m_RenderTargets;

		// depth stencil view
		ID3D11DepthStencilView* m_DepthStencilVeiw;
		// state
		ID3D11DepthStencilState* m_DepthStencilState;

		// view port
		D3D11_VIEWPORT m_ViewPort;

		ID3D11RasterizerState* m_pRasterizerState;

		ID3D11BlendState* m_pBlendState;

		// Stencil reference
		unsigned int m_StencilRef;
		float m_BlendFactor[4];
		unsigned int m_SampleMask;
		float m_ClearColor[4];
		
		// render queue
		std::queue<RenderItem> m_RenderQueueMain;
		std::queue<RenderItem> m_RenderQueueTrsp;

		ID3D11Device* m_pDevice;
		ID3D11DeviceContext* m_pContext;

		void PushRenderQueue( SceneObj* scene, const glm::mat4& modelTrans);

	public:
		Pipeline(ID3D11Device* pDevice, ID3D11DeviceContext* Context);
		virtual ~Pipeline() = default;

		void AddRenderTaget(ID3D11RenderTargetView* renderTarget);
		void SetDepthStencilView(ID3D11DepthStencilView* depthStencilView);
		void ClearRenderTarget();
		void SetRasterizerState(ID3D11RasterizerState* rasterizerState);
		void SetDepthStencilState(ID3D11DepthStencilState* depthStencilState);
		void SetViewPort(const D3D11_VIEWPORT* viewPort);

		virtual void Render(CameraObj* camera, SceneObj* scene);
	};
}
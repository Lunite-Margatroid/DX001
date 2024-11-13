/*
* the pipeline to render normal object and skybox
* 
* excepted to call the methord below before rendering
* 
* void AddRenderTaget(ID3D11RenderTargetView* renderTarget);
* void SetDepthStencilView(ID3D11DepthStencilView* depthStencilView);
* void SetViewPort(const D3D11_VIEWPORT* viewPort);
*/

#pragma once
#include "Pipeline.h"

namespace yoi
{
	class PipelineV1 : public Pipeline
	{
	protected:
		// DS state for skybox rendering
		ID3D11DepthStencilState* m_pDSStateSkybox;

		// render queue for skybox
		RenderQueue m_RenderQueueSkyb;

		// overlay the derived class 
		void PushRenderQueue(SceneObj* scene, const glm::mat4& modelTrans);

	public:
		PipelineV1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		PipelineV1(const PipelineV1&) = delete;
		PipelineV1& operator = (const PipelineV1&) = delete;
		virtual ~PipelineV1();

		virtual void Render(CameraObj* camera, SceneObj* scene) override;

	};
}
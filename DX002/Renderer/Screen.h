#pragma once
#include "Pipeline.h"
#include "Shader\Shader.h"
#include "BufferManager\VertexBuffer.h"
namespace yoi
{
	// explicitly set shader, viewport, render target and depthstencil view to init
	class Screen : public Pipeline
	{
	protected:
		Shader* m_Shader;

		std::unique_ptr<VertexBuffer> m_pVertexBuffer;

		void Init();

	public:
		Screen();
	
		~Screen();
		void Flush();

		virtual void Render(CameraObj* camera, SceneObj* scene);
		void SetShader(Shader* shader);
	};
}
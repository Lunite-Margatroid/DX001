#pragma once
#include "SpriteV3.h"
#include "Texture\UATexture.h"
#include "Texture\CSUTexture.h"
#include "CShader\CShader.h"

namespace yoi
{
	/*
	grid[i-1]
	grid[i]
	grid[i+1]

	bind grid[i-1]
	bind grid[i]
	bind grid[i+1]

	bind compute shader

	undbind

	dispatch thread

	bind sampler
	bind height texture
	bind buffer

	
	*/


	class WaterWave :public SpriteV3
	{
	private:
		std::unique_ptr<UATexture> m_pPreGrid;
		std::unique_ptr<UATexture> m_pCurGrid;

		CSUTexture* m_pHeightTexture;
		CShader* m_UpdateShader;

		unsigned int m_Width;
		float m_CellWidth;
		float m_c;
		float m_Mu;
		float m_tThreshold;
	public:

		WaterWave(CSUTexture* heightTexture, CShader* updateShader, unsigned int width, float cellWidth, float c = 0.05, float mu = 0.5f);
		~WaterWave();

		virtual void RenderV3(const glm::mat4& modelMat, const CameraObj* camera) override;

		void Update(float deltaTime);
	
	};
}
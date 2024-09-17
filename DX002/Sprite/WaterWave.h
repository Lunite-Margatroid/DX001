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
		CShader* m_DisturbShader;
		CShader* m_CalmShader;

		unsigned int m_Width;
		float m_CellWidth;
		float m_c;
		float m_Mu;
		float m_tThreshold;

		float m_DisturbCycle;
		float m_DisturbHeight;
		float m_DisturbTimer;
	public:

		WaterWave(CSUTexture* heightTexture, CShader* updateShader, CShader* disturbShader, CShader* calmShader, unsigned int width, float cellWidth, float c = 0.03f, float mu = 0.4f);
		WaterWave(const WaterWave&) = delete;
		WaterWave& operator = (const WaterWave&) = delete;
		~WaterWave();

		virtual void RenderV3(const glm::mat4& modelMat, const CameraObj* camera) override;

		void Update(float deltaTime) override;

		void Disturb(unsigned int x, unsigned int z, float y);
		
		void RenderImGui() override;

		void WaveReset();
	};
}
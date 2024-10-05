#pragma once
#include "SpriteV3.h"
#include "BufferManager/VertexBuffer.h"
#include "BufferManager/IndexBuffer.h"
#include "Texture\CSUTexture.h"
#include "CShader\CShader.h"
namespace yoi
{
	class QuadCloth : public SpriteV3
	{
		friend class SpriteManager;
	private:
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		Buffer m_cbufferSettings;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pPreGridBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVelRecordBuffer;

		Microsoft::WRL::ComPtr <ID3D11UnorderedAccessView> m_pCurGridView;
		Microsoft::WRL::ComPtr <ID3D11UnorderedAccessView> m_pPreGridView;
		Microsoft::WRL::ComPtr <ID3D11UnorderedAccessView> m_pVelRecordView;

		CSUTexture m_NormalBuffer;

		std::unique_ptr<CShader> m_pUpdateShader;
		std::unique_ptr<CShader> m_pConstraintShader;
		std::unique_ptr<CShader> m_pGenNormal;

		unsigned int m_Width;
		unsigned int m_Height;
		float m_CellWidth;
		float m_CellHeight;
		float m_Alpha;
		float m_Beta;
		float m_ConstDeltaTime;
		float m_Mass;
		float m_G;

		struct ClothSimSettings
		{
			float deltaTime; // cycle time of a frame
			unsigned int nIteration; // num of iterator
			float alpha; // the elastic factor of spring
			float m; // the mass of the particle

			float height; // height of the grid
			float width; // widtd of the grid
			unsigned int nHeight; // cell height
			unsigned int nWidth; // cell width

			float G; // gravitational acceleration
			float tAlpha; // tAlpha = alpha / (deltaTime ^ 2)
			float tBeta;

			unsigned int constraintU;

			unsigned int constraintV;
			float constraintX;
			float constraintY;
			float constraintZ;
		};


		QuadCloth(unsigned int width = 128u, unsigned int height = 128u, float cellWidth = 1.0f / 127.f, float cellHeight = 1.0f / 127.0f , float alpha = 0.5f, 
			float beta = 0.1f, float constDeltaTime = 0.016f, float mass = 1.0f, float G = 9.8f);

	public:
		~QuadCloth();
		QuadCloth(const QuadCloth&) = delete;
		QuadCloth& operator = (const QuadCloth&) = delete;

		void Update(float deltaTime) override;
		

	};
}
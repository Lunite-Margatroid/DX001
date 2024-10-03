#pragma once
#include "SpriteV3.h"
#include "BufferManager/VertexBuffer.h"
#include "BufferManager/IndexBuffer.h"
namespace yoi
{
	class QuadCloth : public SpriteV3
	{
		friend class SpriteManager;
	private:
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pPreGridBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVelRecordBuffer;

		Microsoft::WRL::ComPtr < ID3D11UnorderedAccessView> m_pCurGridView;
		Microsoft::WRL::ComPtr < ID3D11UnorderedAccessView> m_pPreGridView;
		Microsoft::WRL::ComPtr < ID3D11UnorderedAccessView> m_pVelRecordView;


		unsigned int m_Width;
		unsigned int m_Height;
		float m_CellWidth;
		float m_CellHeight;
		float m_Alpha;
		float m_Beta;
		float m_ConstDeltaTime;
		float m_Mass;
		float m_G;

		QuadCloth(unsigned int width = 128u, unsigned int height = 128u, float cellWidth = 1.0f / 127.f, float cellHeight = 1.0f / 127.0f , float alpha = 0.5f, 
			float beta = 0.1f, float constDeltaTime = 0.016f, float mass = 1.0f, float G = 9.8f);

	public:
		~QuadCloth();
		

	};
}
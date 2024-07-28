#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "ImgRes.h"
namespace yoi
{
	class Texture
	{
		friend class Material;
		friend class TextureManager;
	protected:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pResView;
		unsigned int m_Width;
		unsigned int m_Height;
	public:
		Texture(const ImgRes& img);
		// Create a single-color Texture
		Texture(const float* color);
		~Texture() = default;
		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;

		void Bind();
		void Bind(ID3D11DeviceContext* pContext);
	};
}
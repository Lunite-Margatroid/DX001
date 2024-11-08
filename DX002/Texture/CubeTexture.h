/*
* texture for static skybox
*/
#pragma once
#include "Texture.h"

namespace yoi
{
	class CubeTexture : public Texture
	{
	private:
		void Init(ID3D11Device* pDevcie, const D3D11_TEXTURE2D_DESC* td, const D3D11_SUBRESOURCE_DATA* sd);
		CubeTexture() = default;
	public:
		CubeTexture(ID3D11Device* pDevice, unsigned int width, unsigned int height);
		CubeTexture(const CubeTexture&) = delete;
		CubeTexture& operator = (const CubeTexture) = delete;
		~CubeTexture() = default;

		// warning: input ImgRes may be resized
		void SetFaceTexture(ID3D11DeviceContext* pContext, D3D11_TEXTURECUBE_FACE face, ImgRes& img);
	};
}
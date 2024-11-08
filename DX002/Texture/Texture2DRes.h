#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "ImgRes.h"

namespace yoi
{
	class Texture2DRes
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture2D;
		Texture2DRes() = default;

	public:
		Texture2DRes(const ImgRes& res);

		Texture2DRes(const Texture2DRes&) = delete;
		Texture2DRes& operator = (const Texture2DRes) = delete;
		virtual ~Texture2DRes() = default;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> GetTexture2D();

		// not expeted to call the methord. Expected to call GetTexture2D() instead.
		ID3D11Texture2D* GetTexture2DPtr();
	};

	inline unsigned int CalcSubSrc(unsigned int mipSlice, unsigned int arraySlice, unsigned int mipLevels)
	{
		return mipSlice + arraySlice * mipLevels;
	}
}
/*
* Content a Texture2D resource, a ShaderResourceView and an UnorderAccessView.
* Bind() binds ShaderResourceView to Pixel Shader
* BindCS() binds UnorderAccessView to Compute Shader
* BindPS(), BindGS(), BindVS() binds ShaderResouceView to PS, GS and VS.
*/

#pragma once
#include "Texture.h"
#include "ImgRes.h"

namespace yoi
{
	class CSUTexture : public Texture
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_pUAV;

	public:
		/// <summary>
		/// init from img resource
		/// CPU Read and Write is allowed
		/// </summary>
		/// <param name="imgRes"></param>
		CSUTexture(const ImgRes& imgRes);

		/// <summary>
		/// struct a height or position texture. default date is all 0.0
		/// CPU Read and Write is allowed
		/// </summary>
		/// <param name="width">width</param>
		/// <param name="height">height</param>
		/// <param name="format"> denote height texture or positon texture.</param>
		CSUTexture(unsigned int width, unsigned int height, DXGI_FORMAT format = DXGI_FORMAT_R32_FLOAT);

		~CSUTexture() = default;
		CSUTexture(const CSUTexture&) = delete;
		CSUTexture& operator = (const CSUTexture&) = delete;
		

		void InitResourceView();

		/********* defulat bind func ****************/

		virtual void Bind(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void Bind(UINT slot = 0u) override;
		virtual void Unbind(UINT slot = 0u) override;
		virtual void Unbind(ID3D11DeviceContext* pContext, UINT slot = 0u) override;

		/***************************************/

		virtual void BindCS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void BindPS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void BindGS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void BindVS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;

		virtual void UnbindCS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void UnbindPS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void UnbindGS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
		virtual void UnbindVS(ID3D11DeviceContext* pContext, UINT slot = 0u) override;
	};
}

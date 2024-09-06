#pragma once
/*
* Unorder Access View Shader Resource
*/

#include "BasicTexture2D.h"
#include "ImgRes.h"

namespace yoi
{
	class UATexture : public BasicTexture2D
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_pUAV;
	public:
		UATexture() = delete;
		~UATexture() = default;
		UATexture(const UATexture&) = delete;
		UATexture& operator = (const UATexture&) = delete;

		UATexture(const BasicTexture2D& basicTexture);

		/// <summary>
		/// expected struction for RGBA texture
		/// </summary>
		/// <param name=""></param>
		UATexture(ImgRes&);

		/// <summary>
		/// expected struction for height texture
		/// the format is R32_FLOAT
		/// </summary>
		/// <param name="width"> width of the tex </param>
		/// <param name="height"> height of the tex </param>
		/// <param name="data"> data ptr of the resource. all 0 if nullptr is given.</param>
		/// <param name="usage"> usage of the tex </param>
		UATexture( UINT width, UINT height, void* data, D3D11_USAGE usage);

		/********* defulat bind func ****************/

		void Bind(ID3D11DeviceContext* pContext, UINT slot = 0u);
		void Bind(UINT slot = 0u);

		void Unbind(UINT slot = 0u);
		void Unbind(ID3D11DeviceContext* pContext, UINT slot = 0u);

		/******************************************/

		void BindCS(ID3D11DeviceContext* pContext, UINT slot = 0u);
		void BindPS(ID3D11DeviceContext* pContext, UINT slot = 0u);
		void BindGS(ID3D11DeviceContext* pContext, UINT slot = 0u);
		void BindVS(ID3D11DeviceContext* pContext, UINT slot = 0u);

		void UnbindCS(ID3D11DeviceContext* pContext, UINT slot = 0u);
		void UnbindPS(ID3D11DeviceContext* pContext, UINT slot = 0u);
		void UnbindGS(ID3D11DeviceContext* pContext, UINT slot = 0u);
		void UnbindVS(ID3D11DeviceContext* pContext, UINT slot = 0u);

	};
}
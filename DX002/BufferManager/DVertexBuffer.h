#pragma once
#include "VertexBuffer.h"

/*
* set a unordered access view on vertex buffer.
* make vertex buffer dynamic.
* excepted to apply on particle and physical simulation.
* the element type must be float or float vecotr.
*/

namespace yoi
{
	class DVertexBuffer : public VertexBuffer
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_uav;

		unsigned int m_Height;
		unsigned int m_Width;
		unsigned int m_Dimension;
	public:
		DVertexBuffer() = delete;
		DVertexBuffer(const VertexBuffer& vertexBuffer, unsigned int height, unsigned int width, unsigned int dimension) = delete;
		DVertexBuffer(VertexBuffer&& vertexBuffer, unsigned int height, unsigned int width, unsigned int dimension);
		~DVertexBuffer() = default;

		void BindCS(ID3D11DeviceContext* pContext, unsigned int slot);
		void UnbindCS(ID3D11DeviceContext* pContext, unsigned int slot);
	
	};
}
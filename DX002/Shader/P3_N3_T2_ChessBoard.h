#pragma once
#include "Shader.h"

namespace yoi
{
	class P3_N3_T2_ChessBoard : public Shader
	{
	public:
		P3_N3_T2_ChessBoard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
		P3_N3_T2_ChessBoard(const P3_N3_T2_ChessBoard&) = delete;
		P3_N3_T2_ChessBoard& operator = (const P3_N3_T2_ChessBoard&) = delete;
		~P3_N3_T2_ChessBoard() = default;

		const char* GetTitle() const override;
		const char* GetPixelShaderPath() const override; 
		const char* GetVertexShaderPath() const override;
		const char* GetInputLayoutInfo() const override;
	};
}
#pragma once
#include "Renderer\Graphics.h"

class CSGraphics : public yoi::Graphics
{
public:
	CSGraphics(HWND hwnd,unsigned int width, unsigned int height);
	virtual ~CSGraphics() = default;
	CSGraphics(const CSGraphics&) = delete;
	CSGraphics& operator = (const CSGraphics&) = delete;

private:
	std::unique_ptr<yoi::CShader> m_CShader;

	yoi::Texture* m_TestImg;
	yoi::Texture* m_MaskImg;

	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_OutputUAV;
public:
	void ComputeShaderTestInit();
};
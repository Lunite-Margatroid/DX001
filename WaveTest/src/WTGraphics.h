#pragma once
#include "Renderer\Graphics.h"

class WTGraphics : public yoi::Graphics
{
public:
	WTGraphics(HWND hwnd);
	virtual ~WTGraphics() = default;
	WTGraphics(const WTGraphics&) = delete;
	WTGraphics& operator = (const WTGraphics&) = delete;

private:
	std::unique_ptr<yoi::CShader> m_CShader;

	yoi::Texture* m_TestImg;
	yoi::Texture* m_MaskImg;

	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_OutputUAV;
public:
	void ComputeShaderTestInit();
};
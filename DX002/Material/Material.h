#pragma once
#include "Texture\TextureManager.h"
namespace yoi
{
	class Material
	{
	protected:
		Texture* m_texDiffuse;
		Texture* m_texSpecular;
	public:
		Material() = delete;
		Material(TextureManager* texManager);
		Material(const float* texDiffuse, const float* texSpecular, TextureManager* texManager);
		Material(Texture* diffuse, Texture* specular);
		Material(const Material&) = delete;
		~Material() = default;
		Material& operator = (const Material&) = delete;

		void Bind();
		void Bind(ID3D11DeviceContext* pContext);
	};
}
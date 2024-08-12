#pragma once
#include "Texture\TextureManager.h"
namespace yoi
{
	class Material
	{
	protected:
		Texture* m_texDiffuse;
		Texture* m_texSpecular;
		float m_Shininess;
	public:
		Material() = delete;
		Material(TextureManager* texManager, float shininess = 64.0f);
		Material(const float* texDiffuse, const float* texSpecular, TextureManager* texManager, float shininess = 64.0f);
		Material(Texture* diffuse, Texture* specular, float shininess = 64.0f);
		Material(const Material&) = delete;
		~Material() = default;
		Material& operator = (const Material&) = delete;

		void Bind();
		void Bind(ID3D11DeviceContext* pContext);
	};
}
#pragma once
#include "Texture\TextureManager.h"
namespace yoi
{
	class Material
	{
	protected:
		Texture* m_texDiffuse;
		Texture* m_texSpecular;
		Texture* m_texNormal;
		Texture* m_texHeight;
		float m_Shininess;
	public:
		Material() = delete;
		Material(TextureManager* texManager, float shininess = 64.0f);

		/// <summary>
		/// generate single-color texture and set as texture
		/// </summary>
		/// <param name="texDiffuse"> the color of duffuse texture. accepted a vec4 of float</param>
		/// <param name="texSpecular"> the color of specular texture. acceped a vec4 of float </param>
		/// <param name="texManager"> the texture manager of context </param>
		/// <param name="shininess"> the shininess </param>
		Material(const float* texDiffuse, const float* texSpecular, TextureManager* texManager, float shininess = 64.0f);
		Material(Texture* diffuse, Texture* specular, Texture* normal, Texture* height, float shininess = 64.0f);
		Material(Texture* diffuse, TextureManager* texManager);
		Material(Texture* diffuse, Texture* specular, TextureManager* texManager, float shininess = 64.0f);
		Material(const Material&) = delete;
		~Material() = default;
		Material& operator = (const Material&) = delete;

		void Bind();
		void Bind(ID3D11DeviceContext* pContext);

		void Unbind();
		void Unbind(ID3D11DeviceContext* pContext);
	};
}
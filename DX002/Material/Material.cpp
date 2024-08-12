#include "pch.h"
#include "Material.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	Material::Material(TextureManager* texManager, float shininess)
		:m_Shininess(shininess)
	{
		m_texDiffuse = texManager->GetAt(0);
		m_texSpecular = texManager->GetAt(0);
	}
	Material::Material(const float* texDiffuse, const float* texSpecular, TextureManager* texManager, float shininess)
		:m_Shininess(shininess)
	{
		m_texDiffuse = texManager->LoadTexture(texDiffuse);
		m_texSpecular = texManager->LoadTexture(texSpecular);
	}
	Material::Material(Texture* diffuse, Texture* specular, float shininess)
		:m_texDiffuse(diffuse),m_texSpecular(specular), m_Shininess(shininess)
	{
	}
	void Material::Bind()
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(0u, 1u, m_texDiffuse->m_pResView.GetAddressOf()));
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(1u, 1u, m_texSpecular->m_pResView.GetAddressOf()));

		ID3D11Buffer *buf = Graphics::SetBufferData(BufferManager::Buffer::Constant_Material_Shininess, sizeof(float), 0, &m_Shininess);
		GFX_THROW_INFO_ONLY(pContext->VSSetConstantBuffers(1, 1, &buf));
	}
	void Material::Bind(ID3D11DeviceContext* pContext)
	{
		GFX_EXCEPT_SUPPORT();

		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(0u, 1u, m_texDiffuse->m_pResView.GetAddressOf()));
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(1u, 1u, m_texSpecular->m_pResView.GetAddressOf()));
	}
}
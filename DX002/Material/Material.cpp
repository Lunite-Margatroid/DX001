#include "pch.h"
#include "Material.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	Material::Material(TextureManager* texManager, float shininess)
		:m_Shininess(shininess)
	{
		m_texDiffuse = dynamic_cast<Texture*>(texManager->GetAt(0));
		m_texSpecular = dynamic_cast<Texture*>(texManager->GetAt(0));
		m_texNormal = dynamic_cast<Texture*>(texManager->GetAt(1));
		m_texHeight = dynamic_cast<Texture*>(texManager->GetAt(2));
	}
	Material::Material(const float* texDiffuse, const float* texSpecular, TextureManager* texManager, float shininess)
		:m_Shininess(shininess)
	{
		m_texDiffuse = texManager->LoadTexture(texDiffuse);
		m_texSpecular = texManager->LoadTexture(texSpecular);
		m_texNormal = dynamic_cast<Texture*>(texManager->GetAt(1));
		m_texHeight = dynamic_cast<Texture*>(texManager->GetAt(2));
	}
	Material::Material(Texture* diffuse, Texture* specular, Texture* normal , Texture* height, float shininess)
		:m_texDiffuse(diffuse),m_texSpecular(specular), m_texNormal(normal), m_texHeight(height), m_Shininess(shininess)
	{
	}
	Material::Material(Texture* diffuse, TextureManager* texManager)
		:m_texDiffuse(diffuse), 
		m_texSpecular(dynamic_cast<Texture*>(texManager->GetAt(0))), 
		m_texNormal(dynamic_cast<Texture*>(texManager->GetAt(1))), 
		m_texHeight(dynamic_cast<Texture*>(texManager->GetAt(2))), 
		m_Shininess(64.0f)
	{
	}
	Material::Material(Texture* diffuse, Texture* specular, TextureManager* texManager, float shininess)
		:m_texDiffuse(diffuse),
		m_texSpecular(specular),
		m_texNormal(dynamic_cast<Texture*>(texManager->GetAt(1))),
		m_texHeight(dynamic_cast<Texture*>(texManager->GetAt(2))),
		m_Shininess(64.0f)
	{
	}
	void Material::Bind()
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		// bind diffuse texture on pixel shader
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(0u, 1u, m_texDiffuse->m_pResView.GetAddressOf()));
		// bind specular texture on pixel shader
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(1u, 1u, m_texSpecular->m_pResView.GetAddressOf()));

		// bind shininess on pixel shader
		ID3D11Buffer *buf = Graphics::SetBufferData(BufferManager::Buffer::Constant_Material_Shininess, sizeof(float), 0, &m_Shininess);
		GFX_THROW_INFO_ONLY(pContext->PSSetConstantBuffers(1, 1, &buf));

		// bind normal texture on vertex shader
		GFX_THROW_INFO_ONLY(pContext->VSSetShaderResources(2u, 1u, m_texNormal->m_pResView.GetAddressOf()));

		// bind height texture on vertex shader
		GFX_THROW_INFO_ONLY(pContext->VSSetShaderResources(3u, 1u, m_texHeight->m_pResView.GetAddressOf()));

	}
	void Material::Bind(ID3D11DeviceContext* pContext)
	{
		GFX_EXCEPT_SUPPORT();

		// bind diffuse texture on pixel shader
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(0u, 1u, m_texDiffuse->m_pResView.GetAddressOf()));

		// bind specular texture on pixel shader
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(1u, 1u, m_texSpecular->m_pResView.GetAddressOf()));

		// bind shininess const buffer on pixel shader
		ID3D11Buffer* buf = Graphics::SetBufferData(BufferManager::Buffer::Constant_Material_Shininess, sizeof(float), 0, &m_Shininess);
		GFX_THROW_INFO_ONLY(pContext->PSSetConstantBuffers(1, 1, &buf));

		// bind normal texture on vertex shader
		GFX_THROW_INFO_ONLY(pContext->VSSetShaderResources(2u, 1u, m_texNormal->m_pResView.GetAddressOf()));

		// bind height texture on vertex shader
		GFX_THROW_INFO_ONLY(pContext->VSSetShaderResources(3u, 1u, m_texHeight->m_pResView.GetAddressOf()));
	}
	void Material::Unbind()
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();

		ID3D11ShaderResourceView* null_ptr[2] = { nullptr, nullptr };

		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(0u, 2u, null_ptr));
		GFX_THROW_INFO_ONLY(pContext->VSSetShaderResources(2u, 2u, null_ptr));
	}
	void Material::Unbind(ID3D11DeviceContext* pContext)
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11ShaderResourceView* null_ptr[2] = { nullptr, nullptr };

		// unbind diffuse and specular texture
		GFX_THROW_INFO_ONLY(pContext->PSSetShaderResources(0u, 2u, null_ptr));
		// unbind normal and height texture
		GFX_THROW_INFO_ONLY(pContext->VSSetShaderResources(2u, 2u, null_ptr));
	}
}
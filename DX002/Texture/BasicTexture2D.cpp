#include "pch.h"
#include "BasicTexture2D.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
    BasicTexture2D::BasicTexture2D(const BasicTexture2D& other)
        :BasicTexture2D(other.m_pTexture2D)
    {
    }
    BasicTexture2D::BasicTexture2D(BasicTexture2D&& other)
    {
        m_pTexture2D = std::move(other.m_pTexture2D);
    }
    BasicTexture2D& BasicTexture2D::operator=(const BasicTexture2D& other)
    {
        m_pTexture2D.ReleaseAndGetAddressOf();
        m_pTexture2D = other.m_pTexture2D;
        return *this;
    }
    BasicTexture2D& BasicTexture2D::operator=(BasicTexture2D&& other)
    {
        m_pTexture2D.ReleaseAndGetAddressOf();
        m_pTexture2D = std::move(other.m_pTexture2D);
        return *this;
    }
    BasicTexture2D::BasicTexture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture)
        :m_pTexture2D(pTexture)
    {
    }
    BasicTexture2D::BasicTexture2D(const D3D11_TEXTURE2D_DESC* td, const D3D11_SUBRESOURCE_DATA* sd)
    {
        InitTextureResource(td, sd);
    }
    void BasicTexture2D::InitTextureResource(const D3D11_TEXTURE2D_DESC* td, const D3D11_SUBRESOURCE_DATA* sd)
    {
        GFX_EXCEPT_SUPPORT();
        ID3D11Device* pDevice = Graphics::GetInstance().GetDevice();
        GFX_THROW_INFO(pDevice->CreateTexture2D(td, sd, &m_pTexture2D));
    }
    Microsoft::WRL::ComPtr<ID3D11Texture2D> BasicTexture2D::GetTexture2D()
    {
        return m_pTexture2D;
    }
}
#include "pch.h"
#include "CSGraphics.h"
#include "Renderer\GFXMacro.h"
#include "SceneObject\Camera2DObj.h"
#include "CSApplication.h"
#include "Texture\UATexture.h"

CSGraphics::CSGraphics(HWND hwnd)
	:Graphics(hwnd)
{
	ComputeShaderTestInit();
}

void CSGraphics::ComputeShaderTestInit()
{
	GFX_EXCEPT_SUPPORT();

	// craete compute shader
	m_CShader = std::make_unique<yoi::CShader>(pDevice.Get(), "shader-bin/ComputeShader.cso");

	// create test 2d texture
	m_MaskImg = m_pTextureManager->LoadTexture("./img/mask.png");
	m_TestImg = m_pTextureManager->LoadTexture("./img/img.png");
	yoi::Texture* texRumia = m_pTextureManager->LoadTexture("./img/rumia.jpg");

	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// create texture for blended img
	D3D11_TEXTURE2D_DESC td = {};
	td.Width = 256;
	td.Height = 256;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = format;
	td.SampleDesc.Count = 1u;
	td.SampleDesc.Quality = 0u;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	td.CPUAccessFlags = 0u;
	td.MiscFlags = 0u;

	yoi::Texture* tex = m_pTextureManager->LoadTexture(pDevice.Get(), &td, nullptr);
	
	// create unordered access view
	/*D3D11_UNORDERED_ACCESS_VIEW_DESC uavd = {};
	uavd.Format = format;
	uavd.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavd.Texture2D.MipSlice = 0u;

	GFX_THROW_INFO(pDevice->CreateUnorderedAccessView(tex->GetTexture2D().Get(), &uavd, &m_OutputUAV));*/

	yoi::UATexture outputUAV(*tex);

	// bind the resource view of test texture
	// m_MaskImg->Bind(pContext.Get(), 1);
	// m_TestImg->Bind(pContext.Get(), 0);
	m_MaskImg->BindCS(pContext.Get(), 1);
	m_TestImg->BindCS(pContext.Get(), 0);

	// bind unordered access view
	// GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(0, 1, m_OutputUAV.GetAddressOf(), 0u));
	outputUAV.BindCS(pContext.Get(), 0u);
	// m_OutputUAV->Release();


	// bind compute shader
	m_CShader->Bind(pContext.Get());

	// dispatch thread for compute shader
	GFX_THROW_INFO_ONLY(pContext->Dispatch(16, 16, 1));

	// unbind 
	// ID3D11UnorderedAccessView* unbinding = nullptr;
	// GFX_THROW_INFO_ONLY(pContext->CSSetUnorderedAccessViews(0, 1, &unbinding, 0u));
	outputUAV.Unbind(pContext.Get(), 0u);

	// init camera
	yoi::Camera2DObj* camera = new yoi::Camera2DObj();
	camera->SetViewWidth(4);
	camera->SetViewHeight(3);
	m_RootObj->PushChild(dynamic_cast<yoi::SceneObj*>(camera));

	m_MainCamera = dynamic_cast<yoi::CameraObj*>(camera);

	// init sampler
	m_pSampler = std::make_unique<yoi::Sampler>();
	m_pSampler->Bind(pContext.Get());

	// init render object
	// init material
	yoi::Texture* defaultTex = dynamic_cast<yoi::Texture*>(m_pTextureManager->GetAt());
	yoi::Material* mtl = new yoi::Material(
		tex, 
		defaultTex, 
		dynamic_cast<yoi::Texture*>(m_pTextureManager->GetAt(1)),
		dynamic_cast<yoi::Texture*>(m_pTextureManager->GetAt(2)),
		64.0f);
	m_pMaterialManager->Add(mtl, "no name");
	
	yoi::Material* mtlRumia = m_pMaterialManager->CreateMaterial(texRumia, defaultTex, m_pTextureManager.get(), 64.0f);

	// init mesh
	yoi::Mesh mesh(
		m_pBufferManager->GetVertexBuffer(yoi::BufferManager::Buffers::Vertex_P2_T2_Quad),
		m_pBufferManager->GetIndexBuffer(yoi::BufferManager::Buffers::Index_Quad),
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		m_pShaderManager->GetShader("Textured Shader 2D"),
		mtl);

	yoi::Mesh meshRumia(
		m_pBufferManager->GetVertexBuffer(yoi::BufferManager::Buffers::Vertex_P2_T2_Quad),
		m_pBufferManager->GetIndexBuffer(yoi::BufferManager::Buffers::Index_Quad),
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		m_pShaderManager->GetShader("Textured Shader 2D"),
		mtlRumia);
	// init sprite
	yoi::SpriteV3 *spt = m_pSpriteManager->Sprite(mesh);
	yoi::SpriteV3* sptRumia = m_pSpriteManager->Sprite(meshRumia);
	// create obj
	yoi::SceneObj* obj = new yoi::SceneObj(m_RootObj.get(), spt);
	obj = new yoi::SceneObj(m_RootObj.get(), sptRumia, "Rumia");
}

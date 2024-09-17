#include "pch.h"
#include "WTGraphics.h"
#include "Renderer\GFXMacro.h"
#include "SceneObject\Camera2DObj.h"
#include "WTApplication.h"
#include "SceneObject\LightObj.h"

WTGraphics::WTGraphics(HWND hwnd)
	:Graphics(hwnd)
{
	WaveTestInit();
	// InitTestDraw();
}

void WTGraphics::WaveTestInit()
{
	GFX_EXCEPT_SUPPORT();

	//// init camera
	//yoi::Camera2DObj* camera = new yoi::Camera2DObj();
	//camera->SetViewWidth(4);
	//camera->SetViewHeight(3);
	//m_RootObj->PushChild(dynamic_cast<yoi::SceneObj*>(camera));

	yoi::PerspectiveCamera* camera3D = new yoi::PerspectiveCamera(m_RootObj.get(), nullptr, "Camera");
	camera3D->SetWidth(4.0f);
	camera3D->SetHeight(3.0f);
	m_MainCamera = dynamic_cast<yoi::CameraObj*>(camera3D);

	// init sampler
	m_pSampler = std::make_unique<yoi::Sampler>();
	m_pSampler->Bind(pContext.Get(), 0u);

	

	float borderColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	m_pSampler1 = std::make_unique<yoi::Sampler>(
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_BORDER,
		borderColor);
	m_pSampler1->BindVS(pContext.Get(), 1u);

	// create textures
	constexpr float waterColor[4] = { 78.0f / 255.0f, 162.0f / 255.f, 1.0f, 0.2f };
	yoi::Texture* texRumia = m_pTextureManager->LoadTexture("./res/img/rumia.jpg");
	yoi::Texture* texWater = m_pTextureManager->LoadTexture(waterColor, "water face");

	// create object
	yoi::WaterWave* waveSprite = m_pSpriteManager->CreateWave(
		256,
		*m_pShaderManager,
		*m_pBufferManager,
		*m_pTextureManager,
		*m_pMaterialManager,
		texWater);

	yoi::SceneObj* waveObj = new yoi::SceneObj(m_RootObj.get(), waveSprite, "test wave");


	// Add light
	/*m_pLightManager->AddLight(
		yoi::DirLight(
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.1f, 0.7f, 0.7f,
			glm::vec3(0.2f, -0.7f, 0.0f)
		));
	m_pLightManager->Flush();*/

	yoi::Light* pointLight =
		m_pLightManager->AddLight(
			yoi::PointLight()
		);

	
	yoi::Material* mtlRumia = m_pMaterialManager->CreateMaterial(texRumia, m_pTextureManager.get());
	yoi::Material* mtlWhite = m_pMaterialManager->CreateMaterial(dynamic_cast<yoi::Texture*>(m_pTextureManager->GetAt(0)), m_pTextureManager.get());
	yoi::Mesh meshRumia(
		m_pBufferManager->GetBuffer(yoi::BufferManager::Buffer::P3_N3_T2_Cube),
		m_pBufferManager->GetBuffer(yoi::BufferManager::Buffer::Index_Colored_Cube),
		sizeof(float) * 8,
		0,
		0,
		36,
		0,
		0,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		m_pShaderManager->GetShader("Lighted Shader"),
		mtlRumia);
	yoi::Mesh meshWhite(
		m_pBufferManager->GetBuffer(yoi::BufferManager::Buffer::P3_N3_T2_Cube),
		m_pBufferManager->GetBuffer(yoi::BufferManager::Buffer::Index_Colored_Cube),
		sizeof(float) * 8,
		0,
		0,
		36,
		0,
		0,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		m_pShaderManager->GetShader("Texture Color Shader"),
		mtlWhite);
	yoi::SpriteV3* spriteCube = m_pSpriteManager->Sprite(meshRumia);
	yoi::SpriteV3* spriteWhite = m_pSpriteManager->Sprite(meshWhite);
	yoi::SceneObj* objCube = new yoi::SceneObj(m_RootObj.get(), spriteCube, "Rumia Cube");
	yoi::LightObj* objWhite = new yoi::LightObj(m_RootObj.get(), spriteWhite, "White Light", pointLight);
	objWhite->SetScale(glm::vec3(0.3f,0.3f,0.3f));

}

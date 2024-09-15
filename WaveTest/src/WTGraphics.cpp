#include "pch.h"
#include "WTGraphics.h"
#include "Renderer\GFXMacro.h"
#include "SceneObject\Camera2DObj.h"
#include "WTApplication.h"

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
		D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_BORDER,
		borderColor);
	m_pSampler1->Bind(pContext.Get(), 1u);

	// create object
	yoi::WaterWave* waveSprite = m_pSpriteManager->CreateWave(
		256,
		*m_pShaderManager,
		*m_pBufferManager,
		*m_pTextureManager,
		*m_pMaterialManager);

	yoi::SceneObj* waveObj = new yoi::SceneObj(m_RootObj.get(), waveSprite, "test wave");


	// Add light
	m_pLightManager->AddLight(
		yoi::DirLight(
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.1f, 0.7f, 0.7f,
			glm::vec3(0.2f, -0.7f, 0.0f)
		));
	m_pLightManager->Flush();


	yoi::Texture* texRumia = m_pTextureManager->LoadTexture("./res/img/rumia.jpg");
	yoi::Material* mtlRumia = m_pMaterialManager->CreateMaterial(texRumia, m_pTextureManager.get());
	yoi::Mesh meshRumia(
		m_pBufferManager->GetBuffer(yoi::BufferManager::Buffer::P3_N3_T2_Cube),
		m_pBufferManager->GetBuffer(yoi::BufferManager::Buffer::Index_Colored_Cube),
		sizeof(float) * 8 ,
		0,
		0,
		36,
		0,
		0,
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		m_pShaderManager->GetShader("Lighted Shader"),
		mtlRumia);
	yoi::SpriteV3* spriteCube = m_pSpriteManager->Sprite(meshRumia);
	yoi::SceneObj* objCube = new yoi::SceneObj(m_RootObj.get(), spriteCube, "Rumia Cube");

}

#include "pch.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"
#include "SceneObject\LightObj.h"

namespace yoi
{
	void Graphics::InitTestDraw()
	{
		GFX_EXCEPT_SUPPORT();

		//// init camera
		//yoi::Camera2DObj* camera = new yoi::Camera2DObj();
		//camera->SetViewWidth(4);
		//camera->SetViewHeight(3);
		//m_RootObj->PushChild(dynamic_cast<yoi::SceneObj*>(camera));

		yoi::PerspectiveCamera* camera3D = new yoi::PerspectiveCamera(m_RootObj.get(), nullptr, "Camera");
		camera3D->SetWidth(16.0f);
		camera3D->SetHeight(9.0f);
		m_MainCamera = dynamic_cast<yoi::CameraObj*>(camera3D);

		// init sampler
		m_pSampler = std::make_unique<yoi::Sampler>();
		m_pSampler->Bind(pContext.Get(), 0u);

		float borderColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_pSampler1 = std::make_unique<yoi::Sampler>(
			D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_BORDER,
			borderColor);
		m_pSampler1->BindVS(pContext.Get(), 1u);

		// create textures
		constexpr float waterColor[4] = { 78.0f / 255.0f, 162.0f / 255.f, 1.0f, 0.2f };
		yoi::Texture* texRumia = m_pTextureManager->LoadTexture("./res/img/rumia.jpg");
		yoi::Texture* texWater = m_pTextureManager->LoadTexture(waterColor, "water face");

		// create material
		yoi::Material* mtlRumia = m_pMaterialManager->CreateMaterial(texRumia, m_pTextureManager.get());
		yoi::Material* mtlWhite = m_pMaterialManager->CreateMaterial(dynamic_cast<yoi::Texture*>(m_pTextureManager->GetAt(0)), m_pTextureManager.get());


		// create meshes

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

		yoi::Mesh meshQuad(
			m_pBufferManager->GetBuffer(yoi::BufferManager::Buffer::Vertex_P3_N3_T2_Quad),
			m_pBufferManager->GetBuffer(yoi::BufferManager::Buffer::Index_Quad),
			sizeof(float) * 8,
			0u,
			0u,
			4u,
			0u,
			0u,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			m_pShaderManager->GetShader("Lighted Shader"),
			mtlWhite);

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

		// create sprite
		yoi::SpriteV3* spriteCube = m_pSpriteManager->Sprite(meshRumia);
		yoi::SpriteV3* spriteWhite = m_pSpriteManager->Sprite(meshWhite);

		yoi::WaterWave* waveSprite = m_pSpriteManager->CreateWave(
			256,
			*m_pShaderManager,
			*m_pBufferManager,
			*m_pTextureManager,
			*m_pMaterialManager,
			texWater);

		yoi::SpriteV3* spriteQuad = m_pSpriteManager->Sprite(meshQuad);

		// create object

		yoi::SceneObj* objCube = new yoi::SceneObj(m_RootObj.get(), spriteCube, "Rumia Cube");
		objCube->SetVisible(false);

		yoi::LightObj* objWhite = new yoi::LightObj(m_RootObj.get(), spriteWhite, "White Light", pointLight);
		objWhite->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));
		objWhite->SetPosition(glm::vec3(0.5f, 1.0f, 0.5f));



		yoi::SceneObj* objSpriteLeft = new yoi::SceneObj(m_RootObj.get(), spriteQuad, "left quad");
		objSpriteLeft->SetPosition(glm::vec3(-1.0f, 0.0f, 0.0f));
		objSpriteLeft->SetRoll(-PI / 2.0f);

		yoi::SceneObj* objSpriteRight = new yoi::SceneObj(m_RootObj.get(), spriteQuad, "right quad");
		objSpriteRight->SetPosition(glm::vec3(1.0f, 0.0f, 0.0f));
		objSpriteRight->SetRoll(PI / 2.0f);

		yoi::SceneObj* objSpriteBottom = new yoi::SceneObj(m_RootObj.get(), spriteQuad, "bottom quad");
		objSpriteBottom->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));

		yoi::SceneObj* objSpriteFront = new yoi::SceneObj(m_RootObj.get(), spriteQuad, "front quad");
		objSpriteFront->SetPosition(glm::vec3(0.0f, 0.0f, -1.0f));
		objSpriteFront->SetPitch(PI / 2.0f);

		yoi::SceneObj* objSpriteBack = new yoi::SceneObj(m_RootObj.get(), spriteQuad, "back quad");
		objSpriteBack->SetPosition(glm::vec3(0.0f, 0.0f, 1.0f));
		objSpriteBack->SetPitch(-PI / 2.0f);

		yoi::SceneObj* waveObj = new yoi::SceneObj(m_RootObj.get(), waveSprite, "test wave");
		waveObj->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
	}

}
#include "pch.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"
#include "Sprite\ColoredCube.h"
#include "Texture/CubeTexture.h"
#include "Sprite\SpriteSkybox.h"
namespace yoi
{
	void Graphics::InitTestDraw()
	{

		ImgRes img("./img/rumia.jpg");
		m_pTexture = std::make_unique<Texture>(img);
		m_pTexture->Bind();

		Texture* texRumia = m_pTextureManager->LoadTexture("./img/rumia.jpg");

		img.Resize(1024, 1024);
		CubeTexture* texSkybox = m_pTextureManager->CreateTexture<CubeTexture>(pDevice.Get(), 1024, 1024);
		for (unsigned int face = D3D11_TEXTURECUBE_FACE_POSITIVE_X ; face <= D3D11_TEXTURECUBE_FACE_NEGATIVE_Z; face += 1)
		{
			texSkybox->SetFaceTexture(pContext.Get(), static_cast<D3D11_TEXTURECUBE_FACE>(face), img);
		}

		Material* mtlRumia = new Material(texRumia, m_pTextureManager.get());
	
		m_pMaterialManager->Add(mtlRumia, "Rumia");
		Mesh mshRumia(
			GetVertexBuffer(BufferManager::Buffers::Vertex_Textured_Cube),
			GetIndexBuffer(BufferManager::Buffers::Index_Textured_Cube),
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			GetShader("Materialed Shader"),
			mtlRumia);

		Mesh lightedCube(
			GetVertexBuffer(BufferManager::Buffers::P3_N3_T2_Cube),
			GetIndexBuffer(BufferManager::Buffers::Index_Textured_Cube),
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			GetShader("Lighted Shader"),
			mtlRumia);

		// Add light
		m_pLightManager->AddLight(
			PointLight(glm::vec3(1.0f), 0.1f, 0.7f, 0.7f, glm::vec3(3.0f, 4.0f, 0.0f))
		);
		m_pLightManager->AddLight(
			DirLight());
		m_pLightManager->AddLight(
			SpotLight(glm::vec3(1.0f), 0.1f, 0.7f, 0.7f, glm::vec3(-3.0f, 4.0f, 0.0f), 1.0f, 0.14f, 0.07f,
				glm::vec3(1.0f, 0.0f, 0.0f))
		);
		m_pLightManager->AddLight(
			SpotLight(glm::vec3(1.0f), 0.1f, 0.7f, 0.7f, glm::vec3(0.0f, 4.0f, -3.0f), 1.0f, 0.14f, 0.07f,
				glm::vec3(0.0f, 0.0f, 1.0f))
		);

		m_pLightManager->Flush();

		// white material
		Material* whtMaterial = m_pMaterialManager->CreateMaterial(dynamic_cast<Texture*>(m_pTextureManager->GetAt()), dynamic_cast<Texture*>(m_pTextureManager->GetAt()), m_pTextureManager.get(), 64.0f);


		// colored cube
		SpriteV3* cubeSprite = m_pSpriteManager->Sprite(ColoredCube());
		SceneObj* colorCube = new SceneObj(m_RootObj.get(), cubeSprite, "Colored Cube");
		// textured cube
		cubeSprite = m_pSpriteManager->Sprite(lightedCube);
		SceneObj* texturedCube = new SceneObj(m_RootObj.get(), cubeSprite, "Texture Cube");
		texturedCube->SetPosition(glm::vec3(0.0f, 4.0f, 0.0f));

		// gird test
		SpriteV3* gridSprite = m_pSpriteManager->CreateGrid(
			100, 100, m_pShaderManager->GetShader("Programmatic Texture - Chess Board"), whtMaterial, *(m_pBufferManager.get()),
			[](float x, float z) { return 0.3f * (z * sinf(10.f * x) + x * cosf(10.f * z)); },
			[](float x, float z) { return
			glm::normalize(
				glm::vec3(
					-3.f * z * cosf(10.f * x) - 0.3f * cosf(10.f * z),
					1.0f,
					-0.3f * sinf(10.f * x) + 3.f * x * sinf(10.f * z)
				)
			); }
		);
		SceneObj* gridObj = new SceneObj(m_RootObj.get(), gridSprite, "grid");
		gridObj->SetPosition(glm::vec3(4.0f, 0.0f, 0.0f));

		// sampler
		m_pSampler = std::make_unique<Sampler>();
		m_pSampler->Bind(pContext.Get());
		// camera
		m_MainCamera = new PerspectiveCamera(m_RootObj.get(), nullptr, "Camera");
		m_MainCamera->SetPosition(glm::vec3(0.0f, 0.0f, 5.f));
		((PerspectiveCamera*)m_MainCamera)->SetHeight(9.0);
		((PerspectiveCamera*)m_MainCamera)->SetWidth(16.0);

		// skybox sprite
		SpriteSkybox* spriteSkybox = m_pSpriteManager->CreateSprite<SpriteSkybox>(
			texSkybox,
			m_pShaderManager->GetShader("Skybox Shader")
		);

		SceneObj* objSkybox = new SceneObj(m_RootObj.get(), spriteSkybox, "skybox");
	}
}
#include "pch.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"
#include "Sprite\ColoredCube.h"
#include "SceneObject\LightObj.h"

namespace yoi
{
	void Graphics::InitTestDraw()
	{
		float clearColor [4]= {0.0f, 0.0f, 0.0f, 1.f};
		memcpy(m_ClearColor, clearColor, sizeof(float) * 4);
		// Add light
		Light * pointLight = m_pLightManager->AddLight(
			PointLight(glm::vec3(1.0f), 0.1f, 0.7f, 0.7f, glm::vec3(3.0f, 4.0f, 0.0f))
		);
		PointLight* pL = dynamic_cast<PointLight*>(pointLight);
		if (pL)
		{
			pL->SetAttenuationFactor(1, 0.05f);
			pL->SetAttenuationFactor(2, 0.007f);
		}

		m_pLightManager->Flush();

		// create material
		Material* whtMaterial = m_pMaterialManager->CreateMaterial(dynamic_cast<Texture*>(m_pTextureManager->GetAt()), dynamic_cast<Texture*>(m_pTextureManager->GetAt()), m_pTextureManager.get(), 64.0f);


		// sampler
		m_pSampler = std::make_unique<Sampler>();
		m_pSampler->Bind(pContext.Get());

		// camera
		m_MainCamera = new PerspectiveCamera(m_RootObj.get(), nullptr, "Camera");
		m_MainCamera->SetPosition(glm::vec3(5.0f,5.7f, 4.f));
		m_MainCamera->SetEulerAngle(0.75f, -0.55f, 0.f);
		((PerspectiveCamera*)m_MainCamera)->SetHeight(9.0);
		((PerspectiveCamera*)m_MainCamera)->SetWidth(16.0);

		
		
		// create mesh
		Mesh floorMesh(m_pBufferManager->GetVertexBuffer(BufferManager::Buffers::Vertex_P3_N3_T2_Quad),
			m_pBufferManager->GetIndexBuffer(BufferManager::Buffers::Index_Quad),
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			m_pShaderManager->GetShader("Programmatic Texture - Chess Board"),
			whtMaterial);

		Mesh cubeMesh(
			GetVertexBuffer(BufferManager::Buffers::P3_N3_T2_Cube),
			GetIndexBuffer(BufferManager::Buffers::Index_Textured_Cube),
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			GetShader("Texture Color Shader"),
			whtMaterial);

		// create sprite
		SpriteV3* floorSprite = m_pSpriteManager->Sprite(floorMesh);

		QuadCloth::QuadClothDesc desc;
		desc.Alpha = 0.2f;
		desc.G = 9.8f;
		desc.CellHeight = 0.1f;
		desc.CellWidth = 0.1f;
		desc.Mass = 0.0005f;
		desc.Width = 32;
		desc.Height = 32;
		desc.ConstDeltaTime = 0.005f;

		QuadCloth* quadCloth = m_pSpriteManager->CreateQuadCloth(m_pShaderManager->GetShader("Cloth Simulation Shader"), whtMaterial,
			&desc);

		SpriteV3* cubeSprite = m_pSpriteManager->Sprite(cubeMesh);

		// create object
		SceneObj* quadClothObj = new SceneObj(m_RootObj.get(), static_cast<SpriteV3*>(quadCloth), "QuadClothTest");
		SceneObj* floorObj = new SceneObj(m_RootObj.get(), floorSprite, "floor");
		LightObj* pointLightObj = new LightObj(m_RootObj.get(), cubeSprite, "light", pointLight);

		floorObj->SetScale(glm::vec3(8.0f, 1.0f, 8.0f));
		quadClothObj->SetPosition(glm::vec3(0.0f, 4.0f, 0.0f));
		pointLightObj->SetPosition(glm::vec3(1.3f, 4.1f, 2.4f));
		pointLightObj->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));


	}
}
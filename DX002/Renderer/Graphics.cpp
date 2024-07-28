#include "pch.h"
#include <d3dcompiler.h>
#include "Graphics.h"
#include "GFXMacro.h"
#include "Logger\FileLogger.h"
#include "Shader\P3_C3.h"
#include "Sprite\ColoredCube.h"
#include "Sprite\TexturedCube.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")



namespace yoi
{
	/****** singleton ******/
	Graphics* Graphics::s_pInstance = nullptr;
	/*------------------ HrException -----------------------*/
	Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs) noexcept
		:Exception(line, file), hr(hr)
	{
		for (const auto& m : infoMsgs)
		{
			info += m;
			info.push_back('\n');
		}
		if (!info.empty())
		{
			info.pop_back();
		}
	}

	const char* Graphics::HrException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Error String] " << GetErrorString() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl
			<< "[info] " << GetErrorInfo()
			<< GetOriginString();
		whatBuffer = oss.str();
		FileLogger::Error(whatBuffer.c_str());
		FileLogger::Flush();
		return whatBuffer.c_str();
	}

	const char* Graphics::HrException::GetType() const noexcept
	{
		return "Graphics HRESULT Error";
	}

	HRESULT Graphics::HrException::GetErrorCode() const noexcept
	{
		return hr;
	}

	std::string Graphics::HrException::GetErrorString() const noexcept
	{
		return TranslateErrorCode(hr);
	}

	std::string Graphics::HrException::TranslateErrorCode(HRESULT hr) noexcept
	{
		char* pMsgBuf = nullptr;
		DWORD nMsgLen = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			reinterpret_cast<LPSTR>(&pMsgBuf),
			0,
			nullptr);
		if (nMsgLen == 0)
		{
			return "Unidentified error code";
		}
		std::string errorString = pMsgBuf;
		LocalFree(pMsgBuf);
		return errorString;
	}

	std::string Graphics::HrException::GetErrorDescription() const noexcept
	{
		return std::string("");
	}

	std::string Graphics::HrException::GetErrorInfo() const noexcept
	{
		return info;
	}

	/*------------------ HrException ---------------- * end */

	/* ---------------- DeviceRemovedException -------------------*/
	const char* Graphics::DeviceRemovedException::GetType() const noexcept
	{
		return "Device Removed Exception";
	}

	const char* Graphics::DeviceRemovedException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "[Error Code] " << GetErrorCode() << std::endl
			<< "[Error String] " << GetErrorString() << std::endl
			<< "[Description] " << GetErrorDescription() << std::endl
			<< "[info] " << GetErrorInfo()
			<< "[reason] " << reason
			<< GetOriginString();
		whatBuffer = oss.str();
		FileLogger::Error(whatBuffer.c_str());
		FileLogger::Flush();
		return whatBuffer.c_str();
	}
	/* ---------------- DeviceRemovedException ---------------- * end */

	/******************* InfoException ********************/
	Graphics::InfoException::InfoException(int line, const char* file,std::vector<std::string> infoMsgs) noexcept
		:Exception(line, file)
	{
		for (const auto& m : infoMsgs)
		{
			info += m;
			info.push_back('\n');
		}
		if (!info.empty())
		{
			info.pop_back();
		}
	}

	const char* Graphics::InfoException::what() const noexcept
	{
		std::ostringstream oss;
		oss << GetType() << std::endl
			<< "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
		oss << GetOriginString();
		whatBuffer = oss.str();
		FileLogger::Error(whatBuffer.c_str());
		FileLogger::Flush();
		return whatBuffer.c_str();
	}
	const char* Graphics::InfoException::GetType() const noexcept
	{
		return "Graphics Info";
	}

	std::string Graphics::InfoException::GetErrorInfo() const noexcept
	{
		return info;
	}
	/******************* InfoException ************** end */

	Graphics::Graphics(HWND hWnd)
	{
		DXGI_SWAP_CHAIN_DESC scd;

		scd.BufferDesc.Width = 0u;
		scd.BufferDesc.Height = 0u;
		scd.BufferDesc.RefreshRate.Numerator = 0u;
		scd.BufferDesc.RefreshRate.Denominator = 0u;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		scd.SampleDesc.Count = 1;	// multisample count
		scd.SampleDesc.Quality = 0;	// quality level

		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;		// num of buffer in swap chain
		scd.OutputWindow = hWnd;	// window of output
		scd.Windowed = true;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = 0;

		UINT swapCreateFlags = 0u;
#ifdef YOI_DEBUG
		swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		HRESULT hr; // 在macro中使用
		GFX_THROW_FAILED(
		D3D11CreateDeviceAndSwapChain(
			// 1* video adapter
			nullptr, 
			// 2* Driver Type
			D3D_DRIVER_TYPE_HARDWARE,	// primary driver
			// 3* a handle to DLL
			// for software
			nullptr, 
			// 4* flags
			swapCreateFlags,
			// 5* d3d feature levels
			nullptr, 
			// 6* nums of d3d feature levels
			0,
			// 7* SDK Version
			D3D11_SDK_VERSION,
			// 8* swap chain description
			&scd,
			// 9* swap chain ptr
			&pSwap,
			// 10* device ptr
			&pDevice,
			// 11* out
			// return a ptr to D3D_FEATURE_LEVEL
			nullptr, 
			// 12* context ptr
			&pContext
		));

		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
		GFX_THROW_FAILED(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
		GFX_THROW_FAILED(pDevice->CreateRenderTargetView(
			pBackBuffer.Get(),
			nullptr,
			&pTarget));

		// singleton
		s_pInstance = this;

		// Init shader manager
		m_pShaderManager = std::make_unique<ShaderManager>(pDevice.Get(), pContext.Get());

		// buffer manager
		m_pBufferManager = std::make_unique<BufferManager>(pDevice.Get());

		// init texture manager
		m_pTextureManager = std::make_unique<TextureManager>();

		// init material manager
		m_pMaterialManager = std::make_unique<MaterialManager>();

		// init test draw 
		InitTestDraw();

		// init ImGui
		pImGuiManager = std::make_unique<ImGuiManager>(hWnd, pDevice.Get(), pContext.Get());

		m_pObjWin = std::make_unique<ObjectPropertyWin>(m_RootObj.get());

		m_DeltaTime = 0.f;
	}

	void Graphics::ImGuiFrame()
	{
		pImGuiManager->BeginImGuiFrame();

		m_pObjWin->ShowWindow();

		/* temp camera control */
		// key control
		ImGuiIO& io = ImGui::GetIO();
		Camera3DObj* camera = dynamic_cast<Camera3DObj*>(m_MainCamera);
		if (camera)
		{
			if (io.KeysDown[ImGuiKey_W])
				camera->Move(CameraObj::Direction::front, m_DeltaTime);
			if (io.KeysDown[ImGuiKey_A])
				camera->Move(CameraObj::Direction::left, m_DeltaTime);
			if (io.KeysDown[ImGuiKey_S])
				camera->Move(CameraObj::Direction::back, m_DeltaTime);
			if (io.KeysDown[ImGuiKey_D])
				camera->Move(CameraObj::Direction::right, m_DeltaTime);
			if (io.KeysDown[ImGuiKey_LeftShift])
				camera->Move(CameraObj::Direction::down, m_DeltaTime);
			if (io.KeysDown[ImGuiKey_Space])
				camera->Move(CameraObj::Direction::up, m_DeltaTime);
		}
		// mouse control
		ImVec2 windowPos = ImGui::GetCursorScreenPos();					// 当前窗口位置
		ImVec2 windowSize = ImGui::GetWindowSize();					// 窗口大小
		static ImVec2 sLastCursor;
		ImVec2 sDeltaPos;
		if (io.MouseDown[0] && camera)	// 左键按下
		{
			// 这里windowPos 是scene绘制区域的左上角
			if (/*io.MouseClickedPos[0].x >= windowPos.x &&
				io.MouseClickedPos[0].x <= windowPos.x &&
				io.MouseClickedPos[0].y >= windowPos.y &&
				io.MouseClickedPos[0].y <= windowPos.y */
				true)	// 左键点击位置在目标区域内
			{
				sDeltaPos.x = io.MousePos.x - sLastCursor.x;
				sDeltaPos.y = io.MousePos.y - sLastCursor.y;
				// std::cout << "Mouse Drag dleta Pos: " << sDeltaPos.x << ", " << sDeltaPos.y << std::endl;

				camera->RotateYaw(-sDeltaPos.x / 500.0f);
				camera->RotatePitch(-sDeltaPos.y / 500.0f);
			}
		}
		sLastCursor = ImGui::GetMousePos();


	}

	void Graphics::EndFrame()
	{
		HRESULT hr;
#ifdef YOI_DEBUG
		infoManager.Set();
#endif

		pImGuiManager->EndImGuiFrame();

		if (FAILED(hr = pSwap->Present(1u, 0u)))
		{
			if (hr == DXGI_ERROR_DEVICE_REMOVED)
			{
				throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
			}
			else
			{
				GFX_THROW_FAILED(hr);
			}
		}

		
	}
	void Graphics::ClearBuffer(float red, float green, float blue)
	{
		const float color[] = {red, green, blue, 1.0f};
		pContext->ClearRenderTargetView(pTarget.Get(), color);
	}

	void Graphics::InitTestDraw()
	{
		// HRESULT hr;
		// texture
		ImgRes img("./img/rumia.jpg");
		m_pTexture = std::make_unique<Texture>(img);
		m_pTexture->Bind();

		Texture* texRumia = m_pTextureManager->LoadTexture("./img/rumia.jpg");
		Material* mtlRumia = new Material(texRumia, m_pTextureManager->GetAt(0));
		m_pMaterialManager->Add(mtlRumia, "Rumia");
		Mesh mshRumia(
			GetBuffer(BufferManager::Buffer::Vertex_Textured_Cube),
			GetBuffer(BufferManager::Buffer::Index_Textured_Cube),
			sizeof(float) * 5, 
			0u, 
			0u,
			36, 
			0u, 
			0u,
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			GetShader("Materialed Shader"),
			mtlRumia);
		

		// Init Scene obj
		m_RootObj = std::make_unique<SceneObj>(nullptr, nullptr, "Root");

		// colored cube
		SpriteV2* cubeSprite = new SpriteV2();
		cubeSprite->AddMesh(ColoredCube());
		SceneObj* colorCube = new SceneObj(m_RootObj.get(), cubeSprite, "Colored Cube");
		// textured cube
		cubeSprite = new SpriteV2();
		cubeSprite->AddMesh(mshRumia);
		SceneObj* texturedCube = new SceneObj(m_RootObj.get(), cubeSprite, "Texture Cube");
		texturedCube->SetPosition(glm::vec3(0.0f, 4.0f, 0.0f));
		
		// sampler
		m_pSampler = std::make_unique<Sampler>();
		m_pSampler->Bind(pContext.Get());
		// camera
		m_MainCamera = new PerspectiveCamera(m_RootObj.get(), nullptr, "Camera");
		m_MainCamera->SetPosition(glm::vec3(0.0f, 0.0f, 5.f));
		((PerspectiveCamera*)m_MainCamera)->SetHeight(3.0);
		((PerspectiveCamera*)m_MainCamera)->SetWidth(4.0);

		// set viewprot
		D3D11_VIEWPORT vp;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = 800;
		vp.Height = 600;
		vp.MinDepth = 0;
		vp.MaxDepth = 1;
		GFX_THROW_INFO_ONLY(pContext->RSSetViewports(1, &vp));
		// set render target
		GFX_THROW_INFO_ONLY(pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), nullptr));

	}

	void Graphics::DrawTriangle()
	{
		m_DeltaTime = timer.Mark();
		// set render target
		GFX_THROW_INFO_ONLY(pContext->OMSetRenderTargets(1, pTarget.GetAddressOf(), nullptr));

		m_RootObj->Update(m_DeltaTime);
		GFX_THROW_INFO_ONLY(m_RootObj->RenderV2(m_MainCamera->GetVPTrans()));
	}
	ID3D11Device* Graphics::GetDevice()
	{
		return pDevice.Get();
	}

	ID3D11DeviceContext* Graphics::GetContext()
	{
		return pContext.Get();
	}



#ifdef  YOI_DEBUG
	
	DxgiInfoManager& Graphics::GetInfoManager()
	{
		return infoManager;
	}
#endif //  YOI_DEBUG

	
	Graphics& Graphics::GetInstance()
	{
		return *s_pInstance;
	}
	Shader* Graphics::GetShader(const std::string& shaderTitle)
	{
		if(m_pShaderManager.get())
			return m_pShaderManager->GetShader(shaderTitle);
		else
		{
			FileLogger::Error("Shader Manager didn't init.");
			FileLogger::Flush();
			return nullptr;
		}
	}
	ID3D11Buffer* Graphics::GetBuffer(BufferManager::Buffer buffer)
	{
		return (GetInstance().m_pBufferManager)->GetBuffer(buffer);
	}
}
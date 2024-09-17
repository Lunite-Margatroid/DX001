#include "pch.h"
#include "WaterWave.h"
#include "Renderer\Graphics.h"
#include "Renderer\GFXMacro.h"

namespace yoi
{
	WaterWave::WaterWave(CSUTexture* heightTexture, CShader* updateShader, CShader* disturbShader, CShader* calmShader, unsigned int width, float cellWidth, float c, float mu)
		:m_pHeightTexture(heightTexture),
		m_UpdateShader(updateShader),
		m_DisturbShader(disturbShader),
		m_CalmShader(calmShader),
		m_Width(width),
		m_CellWidth(cellWidth),
		m_c(c),
		m_Mu(mu),
		m_DisturbCycle(1.0f),
		m_DisturbHeight(cellWidth),
		m_DisturbTimer(0.0f)
	{
		float temp = 8 * m_c * m_c / (m_CellWidth * m_CellWidth);
		m_tThreshold = (m_Mu + sqrtf(m_Mu * m_Mu + 4 * temp)) / temp;

		D3D11_CPU_ACCESS_FLAG cpuFlag = static_cast<D3D11_CPU_ACCESS_FLAG>(0u);
		m_pPreGrid = std::make_unique<UATexture>
			(m_Width, m_Width, nullptr, cpuFlag);
		m_pCurGrid = std::make_unique<UATexture>
			(m_Width, m_Width, nullptr, cpuFlag);
	}
	WaterWave::~WaterWave()
	{
		if (m_pHeightTexture)
			delete m_pHeightTexture;
	}
	void WaterWave::RenderV3(const glm::mat4& modelMat, const CameraObj* camera)
	{
		// bind water wave update constant buffer
		ID3D11Buffer* buffer = Graphics::GetInstance().GetBuffer(BufferManager::Buffer::Constant_Water_Wave_Update);
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		pContext->VSSetConstantBuffers(2, 1, &buffer);
		// render
		SpriteV3::RenderV3(modelMat, camera);

		// unbind constant buffer
		buffer = nullptr;
		pContext->VSSetConstantBuffers(2, 1, &buffer);
	}
	void WaterWave::Update(float deltaTime)
	{
		if (deltaTime == 0.0f) return;
		if (deltaTime >= m_tThreshold)
			deltaTime = m_tThreshold * 0.7;


		GFX_EXCEPT_SUPPORT();
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();
		
		/********* set water wave updating settings ***********/
		// calculate the coefficients
		float constantSetting[4];

		float temp1 = 2 * deltaTime * deltaTime * m_c * m_c / (m_CellWidth * m_CellWidth);
		float temp2 = m_Mu * deltaTime + 2;

		constantSetting[0] = (4 - 4 * temp1) / temp2;
		constantSetting[1] = temp1 / temp2;
		constantSetting[2] = (m_Mu * deltaTime - 2) / temp2;
		constantSetting[3] = m_CellWidth;

		Graphics::SetBufferData(BufferManager::Buffer::Constant_Water_Wave_Update, sizeof(float) * 4, 0, constantSetting);

		/*********** update the water wave ************/
		// bind pre grid
		m_pPreGrid->BindCS(pContext, 0u);
		m_pCurGrid->BindCS(pContext, 1u);
		// bind output grid
		m_pHeightTexture->BindCS(pContext, 2u);
		// bind constant buffer
		ID3D11Buffer* waveUpdateBuffer = Graphics::GetBuffer(BufferManager::Buffer::Constant_Water_Wave_Update);
		pContext->CSSetConstantBuffers(0, 1, &waveUpdateBuffer);

		// bind Compute shader
		m_UpdateShader->Bind(pContext);

		int count = m_Width / 16;
		count = m_Width % 16 ? count + 1 : count;
		// run compute shader and update
		pContext->Dispatch(count, count, 1);
		
		/*************** random disturb ******************/
		// updatet timer
		m_DisturbTimer += deltaTime;
		// Disturb wave
		if (m_DisturbTimer > m_DisturbCycle)
		{
			Disturb(rand() % m_Width, rand() % m_Width, m_DisturbHeight);
			m_DisturbTimer = 0.0f;
		}
		/************************************************/

		// copy pregrid
		pContext->CopyResource(m_pPreGrid->GetTexture2D().Get(), m_pCurGrid->GetTexture2D().Get());
		pContext->CopyResource(m_pCurGrid->GetTexture2D().Get(), m_pHeightTexture->GetTexture2D().Get());


		// unbind output gird
		m_pHeightTexture->UnbindCS(pContext, 2u);

		// unbind constant buffer
		waveUpdateBuffer = nullptr;
		pContext->CSSetConstantBuffers(0, 1, &waveUpdateBuffer);

	}
	void WaterWave::Disturb(unsigned int x, unsigned int z, float y)
	{
		// set data
		BYTE tempData[16] = {};
		unsigned int* coordPtr = reinterpret_cast<unsigned int*>(tempData);
		coordPtr[0] = x;
		coordPtr[1] = z;

		float* heightPtr = reinterpret_cast<float*>(tempData + 8);
		*heightPtr = y;

		Graphics::SetBufferData(BufferManager::Buffer::Constant_Disturb, 16, 0, tempData);

		// get context
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();

		// bind constant buffer
		ID3D11Buffer* buffer = Graphics::GetBuffer(BufferManager::Buffer::Constant_Disturb);
		pContext->CSSetConstantBuffers(0, 1, &buffer);

		// bind Computer shader
		m_DisturbShader->Bind(pContext);

		// disturb
		pContext->Dispatch(1, 1, 1);
		

	}
	void WaterWave::RenderImGui()
	{
		ImGui::SeparatorText("Water Wave");
		if (ImGui::Button("reset"))
		{
			WaveReset();
		}
		ImGui::Text("width: %d\tcellwidth: %f.3", m_Width, m_CellWidth);
		ImGui::SliderFloat("vel(c)", &m_c, 0.0001f, 1.0f);
		ImGui::SliderFloat("mu", &m_Mu, 0.001f, 0.9f);
		ImGui::SliderFloat("Disturb Height", &m_DisturbHeight, 0.f, 2.f);
		ImGui::SliderFloat("Disturb Cycle", &m_DisturbCycle, 0.1f, 100.f);
	}
	void WaterWave::WaveReset()
	{
		GFX_EXCEPT_SUPPORT();
		ID3D11DeviceContext* pContext = Graphics::GetInstance().GetContext();

		m_pPreGrid->BindCS(pContext, 0u);
		m_pCurGrid->BindCS(pContext, 1u);

		int count = m_Width / 16;
		count = m_Width % 16 ? count + 1 : count;

		m_CalmShader->Bind(pContext);

		pContext->Dispatch(count, count, 1);

	}
}
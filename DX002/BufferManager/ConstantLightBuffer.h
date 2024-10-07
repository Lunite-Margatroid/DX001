#pragma once
#include "Light\Light.h"
void ConstantLightBuffer(yoi::BufferManager* bufMng)
{
	void* tempdata = new unsigned char[128 * LIGHT_MAX_COUNT];
	memset(tempdata, 0, 128 * LIGHT_MAX_COUNT);
	
	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = tempdata;
	data.SysMemPitch = 0u;
	data.SysMemSlicePitch = 0u;

	// direction light
	bufMng->AddConstBuffer(yoi::BufferManager::Buffers::Constant_DirLight, 
		bufMng->m_pDevice, SIZE_DIRETION_LIGHT * LIGHT_MAX_COUNT + 16, &data);

	// point light
	bufMng->AddConstBuffer(yoi::BufferManager::Buffers::Constant_PointLight,
		bufMng->m_pDevice, SIZE_POINT_LIGHT * LIGHT_MAX_COUNT + 16, &data);

	// spot light
	bufMng->AddConstBuffer(yoi::BufferManager::Buffers::Constant_SpotLight,
		bufMng->m_pDevice, SIZE_SPOT_LIGHT * LIGHT_MAX_COUNT + 16, &data);

	delete[] tempdata;
}
#pragma once
#include "Light\Light.h"
void ConstantLightBuffer(yoi::BufferManager* bufMng)
{
	void* tempdata = new unsigned char[128 * LIGHT_MAX_COUNT];
	memset(tempdata, 0, 128 * LIGHT_MAX_COUNT);
	// direction light
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = SIZE_DIRETION_LIGHT * LIGHT_MAX_COUNT + 16;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0u;
	desc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA data = {};
	data.pSysMem = tempdata;
	data.SysMemPitch = 0u;
	data.SysMemSlicePitch = 0u;

	bufMng->AddBuffer(yoi::BufferManager::Buffer::Constant_DirLight, &desc, &data);

	// point light
	desc.ByteWidth = SIZE_POINT_LIGHT * LIGHT_MAX_COUNT + 16;
	bufMng->AddBuffer(yoi::BufferManager::Buffer::Constant_PointLight, &desc, &data);

	// spot light
	desc.ByteWidth = SIZE_SPOT_LIGHT * LIGHT_MAX_COUNT + 16;
	bufMng->AddBuffer(yoi::BufferManager::Buffer::Constant_SpotLight, &desc, &data);

	delete[] tempdata;
}
#include "pch.h"
#include "PointLight.h"
#include "SceneObject/LightObj.h"
namespace yoi
{
	PointLight::PointLight(const glm::vec3& color, float ambient, float diffuse, float specular, const glm::vec3& position, float kConstant, float kLinear, float kQuadratic, LightObj* obj)
		:Light(color, ambient, diffuse, specular, obj),
		m_Position(position),
		m_kConstant(kConstant),
		m_kLinear(kLinear),
		m_kQuadratic(kQuadratic)
	{
	}
	size_t PointLight::WriteToBuffer(void* dest, size_t offset)
	{
		//struct PointLight
		//{
		//	float3 Color;
		//	float3 Contribution;
		//	float3 Position;	// 44 B

		//	float kConstant;    // 48byte
		//	float kLinear;      // 52byte
		//	float kQuadratic;   // 56byte

		//	// 64B ¶ÔÆë
		//};

		size_t size_float3 = sizeof(float) * 3;

		// color
		memcpy(PTR_OFFSET(dest, offset), glm::value_ptr(m_Color), size_float3);
		offset += 16;

		// contribution
		memcpy(PTR_OFFSET(dest, offset), m_Contribution, size_float3);
		offset += 16;

		// position
		memcpy(PTR_OFFSET(dest, offset), glm::value_ptr(m_Position), size_float3);
		offset += 12;

		// k
		memcpy(PTR_OFFSET(dest, offset), &m_kConstant, size_float3);
		offset += 20;

		return offset;
	}
	void PointLight::UpdatePosition()
	{
		if (m_AttachObj)
		{
			m_Position = m_AttachObj->GetModelTrans() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}
	void PointLight::SetAttenuationFactor(int index, float factor)
	{
		if(index >= 0 && index < 3)
			(&m_kConstant)[index] = factor;
	}
	void PointLight::SetPosition(const glm::vec3& position)
	{
		m_Position = position;
	}
}
#include "pch.h"
#include "DirLight.h"
#include "SceneObject\LightObj.h"
namespace yoi
{
	DirLight::DirLight(const glm::vec3& color, float ambient, float diffuse, float specular, const glm::vec3 direction, LightObj* obj)
		:Light(color, ambient, diffuse, specular, obj), m_Direction(direction)
	{
	}
	size_t DirLight::WriteToBuffer(void* dest, size_t offset)
	{
		//struct DirLight
		//{
		//	float3 Color;       // 16byte
		//	float3 Contribution;// 32B
		//	float3 Direction;   // 48byte
		//};

		size_t size_float3 = sizeof(float) * 3;

		// color float3 16B
		memcpy(PTR_OFFSET(dest, offset), glm::value_ptr(m_Color), size_float3);
		offset += 16;

		// Contribution
		memcpy(PTR_OFFSET(dest, offset), m_Contribution, size_float3);
		offset += 16;

		// direction
		memcpy(PTR_OFFSET(dest, offset), glm::value_ptr(m_Direction), size_float3);
		offset += 16;

		return offset;
	}
	void DirLight::UpdatePosition()
	{
		if (m_AttachObj)
		{
			glm::qua<float> qua = m_AttachObj->GetQuaternionRotate();
			glm::mat4 rotationTrans(1.0f);
			SceneObj::QuaternionRotate(rotationTrans, qua);
			m_Direction = rotationTrans * glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
		}
	}
}
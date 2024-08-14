#include "pch.h"
#include "SpotLight.h"

namespace yoi
{
	SpotLight::SpotLight(const glm::vec3& color, float ambient, float diffuse, float specular, const glm::vec3& position, float kConstant, float kLinear, float kQuadratic, const glm::vec3& direction, float outerEdge, float innerEdge)
		:PointLight(color, ambient, diffuse, specular, position, kConstant, kLinear, kQuadratic),
		DirLight(color, ambient, diffuse, specular, direction),
		m_OuterEdge(outerEdge),
		m_InnerEdge(innerEdge)
	{
	}
	size_t SpotLight::WriteToBuffer(void* dest, size_t offset)
	{
		//struct SpotLight
		//{
		//	float3 Color;       // 16B
		//	float3 Contribution;// 32B
		//	float3 Position;    // 48B
		//	float3 Direction;

		//	float outerEdge;    // 64B
		//	float innerEdge;

		//	float kConstant;
		//	float kLinear;
		//	float kQuadratic;   //80 B
		//};
		size_t size_float3 = sizeof(float) * 3;


		// color
		memcpy(PTR_OFFSET(dest, offset), glm::value_ptr(m_Color), size_float3);
		offset += 16;

		// Contribution
		memcpy(PTR_OFFSET(dest, offset), m_Contribution, size_float3);
		offset += 16;

		// Position
		memcpy(PTR_OFFSET(dest, offset), glm::value_ptr(m_Position), size_float3);
		offset += 16;

		// direction
		memcpy(PTR_OFFSET(dest, offset), glm::value_ptr(m_Direction), size_float3);
		offset += 12;

		// edges
		memcpy(PTR_OFFSET(dest, offset), &m_OuterEdge, sizeof(float) * 2);
		offset += 8;

		// k
		memcpy(PTR_OFFSET(dest, offset), &m_kConstant, size_float3);
		offset += 12;

		return offset;
	}
}
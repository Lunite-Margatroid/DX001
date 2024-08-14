#pragma once
#define LIGHT_MAX_COUNT 16
#define SIZE_DIRETION_LIGHT 48
#define SIZE_POINT_LIGHT 64
#define SIZE_SPOT_LIGHT 80
#define PTR_OFFSET(ptr,offset) ((unsigned char*)ptr) + offset
namespace yoi
{
	class Light
	{
	protected:
		glm::vec3 m_Color;
		float m_Contribution[3];

	public:
		Light(const glm::vec3& color = glm::vec3(1.0f), float ambient = 0.1f, float diffuse = 0.7f, float specular = 0.7f);
		Light(const Light&) = default;
		Light& operator = (const Light&) = default;
		virtual ~Light() = default;

		virtual size_t WriteToBuffer(void* dest, size_t offset) = 0;
	};
}
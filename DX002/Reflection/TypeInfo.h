#pragma once

#define MEMBER_OFFSET(thisPtr, member) ((size_t)thisPtr - (size_t)member)
#define GET_MEMBER_PTR(thisPtr, offset) (void*)((size_t)thisPtr + offset)

namespace yoi
{
	class TypeInfo
	{
	public:
		enum class ClassType
		{
			SceneObject, Sprite, Texture, Component
		};
	private:
		ClassType m_ClassType;
		std::string_view m_ClassName;

	public:
		TypeInfo(ClassType classType, const char* className, unsigned int strLen);
		~TypeInfo() = default;
		TypeInfo& operator = (const TypeInfo&) = delete;
		TypeInfo(const TypeInfo&) = delete;
	};

	enum class Numeric
	{
		Int8,
		Int16,
		Int32,
		Int64,

		Uint8, 
		Uint16,
		Uint32,
		Uint64,

		Float32,
		Float64,
		GlmVec2, 
		GlmVec3, 
		GlmVec4
	};

	enum class ComponentProperityType
	{
		Int32,
		Float32,
		GlmVec2,
		GlmVec3,
		GlmVec4,
		vec2,
		vec3,
		vec4
	};

	struct NumberVariable
	{
		unsigned int Offset;
		std::string_view VariableName;
	};

	struct ComponentProperity
	{
		unsigned int Offset;
		ComponentProperityType Type;
		std::string_view VariableName;

		ComponentProperity(unsigned int offset, ComponentProperityType type, const std::string_view varName)
			:Offset(offset),Type(type),VariableName(varName)
		{}
	};
}
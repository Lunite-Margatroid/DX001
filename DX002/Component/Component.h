#pragma once

namespace yoi
{
	class Component
	{
	protected:
		void* m_pAttachedObj;
	public:
		Component();
		Component(const Component&) = delete;
		virtual ~Component() = default;
		Component& operator = (const Component& cmpt) = delete;

		void* GetAttachedObj();
		void* SetAttach(void* attachObj);

		virtual void Update(float deltaTime);


	};
}
#pragma once
#include <map>
#include <functional>
#include "Entity_UUID.h"

namespace GloryECS
{
	class EntityRegistry;

	enum class InvocationType
	{
		OnAdd,
		OnRemove,
		Start,
		Stop,
		Update,
		Draw,

		Count
	};

	template<typename T>
	class ComponentInvokations
	{
	public:
		ComponentInvokations()
		{
			for (size_t i = 0; i < (size_t)InvocationType::Count; i++)
			{
				m_Callbacks[InvocationType(i)] =  NULL;
			}
		}
		virtual ~ComponentInvokations() { m_Callbacks.clear(); }

		void Invoke(const InvocationType& invocationType, EntityRegistry* pRegistry, EntityID entity, T& component)
		{
			if (!m_Callbacks[invocationType]) return;
			m_Callbacks[invocationType](pRegistry, entity, component);
		}

	private:
		friend class EntityRegistry;
		std::map<InvocationType, std::function<void(EntityRegistry*, EntityID, T&)>> m_Callbacks;
	};
}

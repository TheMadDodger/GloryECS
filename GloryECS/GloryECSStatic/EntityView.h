#pragma once
#include <vector>
#include <map>
#include "Entity_UUID.h"

namespace GloryECS
{
	class EntityRegistry;

	class EntityView
	{
	public:
		EntityView(EntityRegistry* pRegistry);

		size_t ComponentCount();
		Glory::UUID ComponentUUIDAt(size_t index);
		size_t ComponentTypeAt(size_t index);
		size_t ComponentType(Glory::UUID uuid);
		std::map<Glory::UUID, size_t>::iterator GetIterator();
		std::map<Glory::UUID, size_t>::iterator GetIteratorEnd();
		void SwapComponentIndex(size_t index1, size_t index2);
		void SetComponentIndex(size_t oldIndex, size_t newIndex);

	private:
		void Add(size_t hash, Glory::UUID uuid = Glory::UUID());
		void Remove(size_t hash);
		void Remove(Glory::UUID uuid);

	private:
		friend class EntityRegistry;
		EntityRegistry* m_pRegistry;
		std::map<Glory::UUID, size_t> m_ComponentTypes;
		std::map<size_t, Glory::UUID> m_TypeToUUID;
		std::vector<Glory::UUID> m_ComponentOrder;
	};
}

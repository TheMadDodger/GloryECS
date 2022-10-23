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
		size_t ComponentTypeAt(size_t index);
		std::map<Glory::UUID, size_t>::iterator GetIterator();
		std::map<Glory::UUID, size_t>::iterator GetIteratorEnd();

	private:
		void Add(size_t hash, Glory::UUID uuid = Glory::UUID());
		void Remove(size_t hash);
		void Remove(Glory::UUID uuid);

	private:
		friend class EntityRegistry;
		EntityRegistry* m_pRegistry;
		std::map<Glory::UUID, size_t> m_ComponentTypes;
		std::map<size_t, Glory::UUID> m_TypeToUUID;
	};
}

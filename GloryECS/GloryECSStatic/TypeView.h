#pragma once
#include "EntityID.h"
#include <vector>
#include <exception>
#include <typeindex>

#define MAX_COMPONENTS 1000

namespace GloryECS
{
	class BaseTypeView
	{
	public:
		BaseTypeView(size_t typeHash);
		virtual ~BaseTypeView();

		void Remove(EntityID entityID);

		bool Contains(EntityID entityID);

	protected:
		virtual void OnRemove(size_t index) = 0;

	protected:
		const size_t m_TypeHash;
		std::vector<EntityID> m_Entities;
	};

	template<typename T>
	class TypeView : public BaseTypeView
	{
	public:
		TypeView() : BaseTypeView(std::hash<std::type_index>()(typeid(T))) {}
		virtual ~TypeView()
		{
			m_ComponentData.clear();
		}

		template<typename... Args>
		T& Add(EntityID entityID, Args&&... args)
		{
			m_Entities.push_back(entityID);
			size_t index = m_ComponentData.size();
			m_ComponentData.push_back(T(args...));
			return m_ComponentData[index];
		}

		virtual void OnRemove(size_t index) override
		{
			m_ComponentData.erase(m_ComponentData.begin() + index);
		}

		T& Get(EntityID entityID)
		{
			auto it = std::find(m_Entities.begin(), m_Entities.end(), entityID);
			if (it == m_Entities.end())
				throw new std::exception("Entity not in view");
			size_t index = it - m_Entities.begin();
			return m_ComponentData[index];
		}

	private:
		std::vector<T> m_ComponentData;
	};
}

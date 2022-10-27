#pragma once
#include "EntityID.h"
#include <vector>
#include <exception>
#include <typeindex>

#define MAX_COMPONENTS 1000

namespace GloryECS
{
	class EntityRegistry;

	class BaseTypeView
	{
	public:
		BaseTypeView(size_t typeHash, EntityRegistry* pRegistry);
		virtual ~BaseTypeView();

		void Remove(EntityID entityID);
		bool Contains(EntityID entityID);
		const size_t ComponentTypeHash() const;
		virtual const std::type_index ComponentType() const = 0;
		virtual void* Create(EntityID entityID) = 0;
		virtual void* GetComponentAddress(EntityID entityID, size_t number = 0) = 0;

	protected:
		virtual void OnRemove(size_t index) = 0;

	protected:
		const size_t m_TypeHash;
		std::vector<EntityID> m_Entities;
		EntityRegistry* m_pRegistry;

	private:
		friend class EntityRegistry;
	};

	template<typename T>
	class TypeView : public BaseTypeView
	{
	public:
		TypeView(EntityRegistry* pRegistry) : BaseTypeView(std::hash<std::type_index>()(typeid(T)), pRegistry) {}
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
		
		virtual const std::type_index ComponentType() const override
		{
			return typeid(T);
		}

		virtual void* Create(EntityID entityID) override
		{
			m_Entities.push_back(entityID);
			size_t index = m_ComponentData.size();
			m_ComponentData.push_back(T());
			return &m_ComponentData[index];
		}

		virtual void* GetComponentAddress(EntityID entityID, size_t number = 0) override
		{
			auto it = std::find_if(m_Entities.begin(), m_Entities.end(), [&](EntityID othereEntity)
			{
				if (othereEntity != entityID) return false;
				if (number != 0)
				{
					--number;
					return false;
				}
				return true;
			});
			size_t index = m_Entities.end() - it;
			return &m_ComponentData[index];
		}

	private:
		std::vector<T> m_ComponentData;
	};
}

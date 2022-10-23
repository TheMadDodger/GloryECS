#pragma once
#include "EntityID.h"
#include "TypeView.h"
#include "EntityView.h"
#include <map>
#include <unordered_map>

namespace GloryECS
{
	class EntityRegistry
	{
	public:
		EntityRegistry();
		virtual ~EntityRegistry();

		EntityID CreateEntity();
		void DestroyEntity(EntityID entity);

		template<typename Component, typename... Args>
		EntityID CreateEntity(Args&&... args)
		{
			EntityID entityID = CreateEntity();
			AddComponent<Component>(entityID, std::forward<Args>(args)...);
			return entityID;
		}

		template<typename Component, typename... Args>
		Component& AddComponent(EntityID entity, Args&&... args)
		{
			TypeView<Component>* pTypeView = GetTypeView<Component>();
			return pTypeView->Add(entity, std::forward<Args>(args)...);
		}

		template<typename Component>
		TypeView<Component>* GetTypeView()
		{
			size_t hash = std::hash<std::type_index>()(typeid(Component));
			if (m_pTypeViews.find(hash) == m_pTypeViews.end())
				m_pTypeViews[hash] = new TypeView<Component>();

			TypeView<Component>* pTypeView = (TypeView<Component>*)m_pTypeViews[hash];
			return pTypeView;
		}

		BaseTypeView* GetTypeView(size_t typeHash)
		{
			if (m_pTypeViews.find(typeHash) == m_pTypeViews.end())
				throw new std::exception("Type does not exist");

			return m_pTypeViews[typeHash];
		}

		template<typename Component>
		bool HasComponent(EntityID entity)
		{
			TypeView<Component>* pTypeView = GetTypeView<Component>();
			return pTypeView->Contains(entity);
		}

		template<typename Component>
		Component& GetComponent(EntityID entity)
		{
			TypeView<Component>* pTypeView = GetTypeView<Component>();
			if (!pTypeView->Contains(entity))
				throw new std::exception("Entity does not have component!");

			return pTypeView->Get(entity);
		}

		//template<typename Component>
		//void RemoveComponent(EntityID entity)
		//{
		//	std::unique_lock<std::mutex> lock(m_EntityComponentsLock);
		//	for (size_t i = 0; i < m_ComponentsPerEntity[entity].size(); i++)
		//	{
		//		size_t index = m_ComponentsPerEntity[entity][i];
		//		if (m_EntityComponents[index].GetType() != typeid(Component)) continue;
		//		m_Systems.OnComponentRemoved(this, entity, &m_EntityComponents[index]);
		//		m_UnusedComponentIndices.push_back(index);
		//		m_ComponentsPerEntity[entity].erase(m_ComponentsPerEntity[entity].begin() + index);
		//		auto it = std::remove(m_ComponentsPerType[typeid(Component)].begin(), m_ComponentsPerType[typeid(Component)].end(), index);
		//		m_ComponentsPerType[typeid(Component)].erase(it);
		//	}
		//	lock.unlock();
		//}

		//void RemoveComponent(EntityID entity, size_t index);
		//void ChangeComponentIndex(EntityID entity, size_t index, size_t newIndex);
		//size_t ComponentCount(EntityID entity);
		//
		//void Clear(EntityID entity);
		//
		//size_t Alive();
		//
		//bool IsValid(EntityID entity);
		//
		//void ForEach(std::function<void(EntityRegistry*, EntityID)> func);
		//void ForEachComponent(EntityID entity, std::function<void(EntityRegistry*, EntityID, EntityComponentData*)> func);
		//
		//EntityComponentData* GetEntityComponentDataAt(EntityID entity, size_t index);
		//
		//template<typename C, typename Component>
		//EntitySystem* RegisterSystem()
		//{
		//	return m_Systems.Register<C, Component>(this);
		//}
		//
		//template<typename Component>
		//EntitySystem* RegisterSystem()
		//{
		//	return m_Systems.Register<Component>(this);
		//}
		//
		//template<typename Component>
		//void ForEach(std::function<void(EntityRegistry*, EntityID, EntityComponentData*)> func)
		//{
		//	ForEach(typeid(Component), func);
		//}
		//
		//void ForEach(const std::type_index& type, std::function<void(EntityRegistry*, EntityID, EntityComponentData*)> func);
		//
		//void Update();
		//bool IsUpdating();
		//void Draw();
		//EntitySystems* GetSystems();

	private:
		// Entity data
		std::map<EntityID, EntityView> m_EntityViews;
		EntityID m_NextEntityID;

		// Basic type views
		std::map<size_t, BaseTypeView*> m_pTypeViews;

		//EntitySystems m_Systems;
	};
}

#pragma once
#include "EntityID.h"
#include "TypeView.h"
#include "EntityView.h"
#include "ComponentReferenceManager.h"
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
		EntityID CreateEntity(Glory::UUID uuid, Args&&... args)
		{
			EntityID entityID = CreateEntity();
			AddComponent<Component>(entityID, uuid, std::forward<Args>(args)...);
			return entityID;
		}

		template<typename Component, typename... Args>
		Component& AddComponent(EntityID entity, Args&&... args)
		{
			TypeView<Component>* pTypeView = GetTypeView<Component>();
			Component& component = pTypeView->Add(entity, std::forward<Args>(args)...);
			EntityView* pEntityView = GetEntityView(entity);
			pEntityView->Add(pTypeView->m_TypeHash);
			return component;
		}

		template<typename Component, typename... Args>
		Component& AddComponent(EntityID entity, Glory::UUID uuid, Args&&... args)
		{
			TypeView<Component>* pTypeView = GetTypeView<Component>();
			Component& component = pTypeView->Add(entity, std::forward<Args>(args)...);
			EntityView* pEntityView = GetEntityView(entity);
			pEntityView->Add(pTypeView->m_TypeHash, uuid);
			return component;
		}

		template<typename Component>
		TypeView<Component>* GetTypeView()
		{
			size_t hash = std::hash<std::type_index>()(typeid(Component));
			if (m_pTypeViews.find(hash) == m_pTypeViews.end())
				m_pTypeViews[hash] = new TypeView<Component>(this);

			TypeView<Component>* pTypeView = (TypeView<Component>*)m_pTypeViews[hash];
			return pTypeView;
		}

		BaseTypeView* GetTypeView(size_t typeHash);
		EntityView* GetEntityView(EntityID entity);

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

		template<typename Component>
		void RemoveComponent(EntityID entity)
		{
			EntityView* pEntityView = GetEntityView(entity);

			TypeView<Component>* pTypeView = GetTypeView<Component>();
			if (!pTypeView->Contains(entity))
				throw new std::exception("Entity does not have component!");

			pTypeView->Remove(entity);
			pEntityView->Remove(pTypeView->m_TypeHash);
		}

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
		std::map<EntityID, EntityView*> m_pEntityViews;
		EntityID m_NextEntityID;

		// Basic type views
		std::map<size_t, BaseTypeView*> m_pTypeViews;

		ComponentReferenceManager m_ReferenceManager;

		//EntitySystems m_Systems;
	};
}

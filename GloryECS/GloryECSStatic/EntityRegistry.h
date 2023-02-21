#pragma once
#include "EntityID.h"
#include "TypeView.h"
#include "EntityView.h"
#include "ComponentTypes.h"
#include <map>
#include <unordered_map>
#include <functional>

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
			const ComponentType* componentType = ComponentTypes::GetComponentType(pTypeView->m_TypeHash);
			if (!componentType->m_AllowMultiple && pTypeView->Contains(entity))
			{
				throw new std::exception(("Duplicate component of type " + componentType->m_Name + " not allowed!").c_str());
			}

			Component& component = pTypeView->Add(entity, std::forward<Args>(args)...);
			EntityView* pEntityView = GetEntityView(entity);
			Glory::UUID uuid;
			pEntityView->Add(pTypeView->m_TypeHash, uuid);
			pTypeView->m_Callbacks.Invoke(InvocationType::OnAdd, this, entity, component);
			return component;
		}

		template<typename Component, typename... Args>
		Component& AddComponent(EntityID entity, Glory::UUID uuid, Args&&... args)
		{
			TypeView<Component>* pTypeView = GetTypeView<Component>();
			const ComponentType* componentType = ComponentTypes::GetComponentType(pTypeView->m_TypeHash);
			if (!componentType->m_AllowMultiple && pTypeView->Contains(entity))
			{
				throw new std::exception(("Duplicate component of type " + componentType->m_Name + " not allowed!").c_str());
			}
			Component& component = pTypeView->Add(entity, std::forward<Args>(args)...);
			EntityView* pEntityView = GetEntityView(entity);
			pEntityView->Add(pTypeView->m_TypeHash, uuid);
			pTypeView->m_Callbacks.Invoke(InvocationType::OnAdd, this, entity, component);
			return component;
		}

		void* CreateComponent(EntityID entityID, size_t typeHash, Glory::UUID uuid);

		template<typename Component>
		TypeView<Component>* GetTypeView()
		{
			size_t hash = Hashing::Hash(typeid(Component).name());
			if (m_pTypeViews.find(hash) == m_pTypeViews.end())
				m_pTypeViews[hash] = new TypeView<Component>(this);

			TypeView<Component>* pTypeView = (TypeView<Component>*)m_pTypeViews[hash];
			return pTypeView;
		}

		BaseTypeView* GetTypeView(size_t typeHash);
		EntityView* GetEntityView(EntityID entity);

		void* GetComponentAddress(EntityID entityID, Glory::UUID componentID);

		template<typename Component>
		bool HasComponent(EntityID entity)
		{
			TypeView<Component>* pTypeView = GetTypeView<Component>();
			return pTypeView->Contains(entity);
		}

		bool HasComponent(EntityID entity, size_t type);

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

			Component& component = pTypeView->Get(entity);
			pTypeView->m_Callbacks.Invoke(InvocationType::OnRemove, this, entity, component);

			pTypeView->Remove(entity);
			pEntityView->Remove(pTypeView->m_TypeHash);
		}

		void RemoveComponent(EntityID entity, size_t typeHash);
		void RemoveComponentAt(EntityID entity, size_t index);
		//void ChangeComponentIndex(EntityID entity, size_t index, size_t newIndex);
		size_t ComponentCount(EntityID entity);
		void Clear(EntityID entity);
		const size_t Alive() const;
		const bool IsValid(EntityID entity) const;
		const size_t TypeViewCount() const;
		std::map<size_t, BaseTypeView*>::iterator GetTypeViewIterator();
		std::map<size_t, BaseTypeView*>::iterator GetTypeViewIteratorEnd();
		
		template<typename T>
		void RegisterInvokaction(InvocationType invocationType, std::function<void(EntityRegistry*, EntityID, T&)> callback)
		{
			TypeView<T>* pTypeView = GetTypeView<T>();
			pTypeView->m_Callbacks.m_Callbacks[invocationType] = callback;
		}

		template<typename T>
		void InvokeAll(InvocationType invocationType)
		{
			TypeView<T>* pTypeView = GetTypeView<T>();
			pTypeView->InvokeAll(invocationType, this);
		}

		void InvokeAll(size_t typeHash, InvocationType invocationType);
		void InvokeAll(InvocationType invocationType);

		//void ForEach(std::function<void(EntityRegistry*, EntityID)> func);
		//void ForEachComponent(EntityID entity, std::function<void(EntityRegistry*, EntityID, size_t, size_t)> func);
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
		friend class ComponentTypes;

		// Entity data
		std::map<EntityID, EntityView*> m_pEntityViews;
		EntityID m_NextEntityID;

		// Basic type views
		std::map<size_t, BaseTypeView*> m_pTypeViews;

		//EntitySystems m_Systems;
	};
}

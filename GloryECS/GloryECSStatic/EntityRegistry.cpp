#include "EntityRegistry.h"
#include "ComponentTypes.h"

namespace GloryECS
{
	EntityRegistry::EntityRegistry() : m_NextEntityID(1)
	{
	}

	EntityRegistry::~EntityRegistry()
	{
		for (auto it = m_pTypeViews.begin(); it != m_pTypeViews.end(); it++)
		{
			delete it->second;
		}
		
		for (auto it = m_pEntityViews.begin(); it != m_pEntityViews.end(); it++)
		{
			delete it->second;
		}

		m_pEntityViews.clear();
		m_pTypeViews.clear();
	}

	EntityID EntityRegistry::CreateEntity()
	{
		EntityID newEntity = m_NextEntityID;
		++m_NextEntityID;
		m_pEntityViews.emplace(newEntity, new EntityView(this));
		return newEntity;
	}

	void EntityRegistry::DestroyEntity(EntityID entity)
	{
		EntityView* pEntityView = GetEntityView(entity);
		for (auto it = pEntityView->m_ComponentTypes.begin(); it != pEntityView->m_ComponentTypes.end(); it++)
		{
			size_t typeHash = it->second;
			BaseTypeView* pTypeView = GetTypeView(typeHash);
			void* pAddress = pTypeView->GetComponentAddress(entity);
			pTypeView->Invoke(InvocationType::OnRemove, this, entity, pAddress);
			pTypeView->Remove(entity);
		}
		delete m_pEntityViews[entity];
		m_pEntityViews.erase(entity);
	}

	void* EntityRegistry::CreateComponent(EntityID entityID, size_t typeHash, Glory::UUID uuid)
	{
		BaseTypeView* pTypeView = GetTypeView(typeHash);
		void* pAddress = pTypeView->Create(entityID);
		EntityView* pEntityView = GetEntityView(entityID);
		pEntityView->Add(pTypeView->m_TypeHash, uuid);
		pTypeView->Invoke(InvocationType::OnAdd, this, entityID, pAddress);
		return pAddress;
	}

	BaseTypeView* EntityRegistry::GetTypeView(size_t typeHash)
	{
		if (m_pTypeViews.find(typeHash) == m_pTypeViews.end())
			return ComponentTypes::CreateTypeView(this, typeHash);

		return m_pTypeViews[typeHash];
	}

	EntityView* EntityRegistry::GetEntityView(EntityID entity)
	{
		if (m_pEntityViews.find(entity) == m_pEntityViews.end())
			throw new std::exception("Entity does not exist");

		return m_pEntityViews[entity];
	}

	void* EntityRegistry::GetComponentAddress(EntityID entityID, Glory::UUID componentID)
	{
		EntityView* pEntityView = GetEntityView(entityID);
		size_t hash = pEntityView->m_ComponentTypes.at(componentID);
		BaseTypeView* pTypeView = GetTypeView(hash);

		// TODO: Get number of the component in case of duplicates
		return pTypeView->GetComponentAddress(entityID);
	}

	void EntityRegistry::RemoveComponent(EntityID entity, size_t typeHash)
	{
		EntityView* pEntityView = GetEntityView(entity);
		BaseTypeView* pTypeView = GetTypeView(typeHash);
		void* pAddress = pTypeView->GetComponentAddress(entity);
		pTypeView->Invoke(InvocationType::OnRemove, this, entity, pAddress);
		pTypeView->Remove(entity);
		m_pEntityViews[entity]->Remove(typeHash);
	}

	void EntityRegistry::RemoveComponentAt(EntityID entity, size_t index)
	{
		EntityView* pEntityView = GetEntityView(entity);
		size_t typeHash = pEntityView->ComponentTypeAt(index);
		BaseTypeView* pTypeView = GetTypeView(typeHash);
		void* pAddress = pTypeView->GetComponentAddress(entity);
		pTypeView->Invoke(InvocationType::OnRemove, this, entity, pAddress);
		pTypeView->Remove(entity);
		m_pEntityViews[entity]->Remove(typeHash);
	}

	size_t EntityRegistry::ComponentCount(EntityID entity)
	{
		EntityView* pEntityView = GetEntityView(entity);
		return pEntityView->ComponentCount();
	}

	void EntityRegistry::Clear(EntityID entity)
	{
		EntityView* pEntityView = GetEntityView(entity);
		for (auto it = pEntityView->m_ComponentTypes.begin(); it != pEntityView->m_ComponentTypes.end(); it++)
		{
			size_t typeHash = it->second;
			BaseTypeView* pTypeView = GetTypeView(typeHash);
			void* pAddress = pTypeView->GetComponentAddress(entity);
			pTypeView->Invoke(InvocationType::OnRemove, this, entity, pAddress);
			pTypeView->Remove(entity);
		}
	}

	const size_t EntityRegistry::Alive() const
	{
		return m_pEntityViews.size();
	}

	const bool EntityRegistry::IsValid(EntityID entity) const
	{
		return m_pEntityViews.find(entity) != m_pEntityViews.end();
	}
	const size_t EntityRegistry::TypeViewCount() const
	{
		return m_pTypeViews.size();
	}

	std::map<size_t, BaseTypeView*>::iterator EntityRegistry::GetTypeViewIterator()
	{
		return m_pTypeViews.begin();
	}

	std::map<size_t, BaseTypeView*>::iterator EntityRegistry::GetTypeViewIteratorEnd()
	{
		return m_pTypeViews.end();
	}

	void EntityRegistry::InvokeAll(size_t typeHash, InvocationType invocationType)
	{
		BaseTypeView* pTypeView = GetTypeView(typeHash);
		pTypeView->InvokeAll(invocationType, this);
	}

	void EntityRegistry::InvokeAll(InvocationType invocationType)
	{
		for (auto it = m_pTypeViews.begin(); it != m_pTypeViews.end(); it++)
		{
			it->second->InvokeAll(invocationType, this);
		}
	}
}

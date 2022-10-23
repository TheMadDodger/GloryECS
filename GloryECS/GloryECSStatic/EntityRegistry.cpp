#include "EntityRegistry.h"

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

	}

	BaseTypeView* EntityRegistry::GetTypeView(size_t typeHash)
	{
		if (m_pTypeViews.find(typeHash) == m_pTypeViews.end())
			throw new std::exception("Type does not exist");

		return m_pTypeViews[typeHash];
	}

	EntityView* EntityRegistry::GetEntityView(EntityID entity)
	{
		if (m_pEntityViews.find(entity) == m_pEntityViews.end())
			throw new std::exception("Entity does not exist");

		return m_pEntityViews[entity];
	}
}

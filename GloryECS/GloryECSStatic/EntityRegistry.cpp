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
		
		//for (auto it = m_EntityViews.begin(); it != m_EntityViews.end(); it++)
		//{
		//	delete it->second;
		//}

		m_EntityViews.clear();
		m_pTypeViews.clear();
	}

	EntityID EntityRegistry::CreateEntity()
	{
		EntityID newEntity = m_NextEntityID;
		++m_NextEntityID;
		m_EntityViews.emplace(newEntity, EntityView(this));
		return newEntity;
	}

	void EntityRegistry::DestroyEntity(EntityID entity)
	{

	}
}

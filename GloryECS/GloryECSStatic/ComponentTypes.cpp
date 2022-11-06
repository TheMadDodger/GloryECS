#include "ComponentTypes.h"
#include "EntityRegistry.h"
#include <exception>

namespace GloryECS
{
	ComponentTypes* ComponentTypes::m_pInstance = nullptr;
	bool ComponentTypes::m_InstanceOwned = false;

	ComponentTypes* ComponentTypes::CreateInstance()
	{
		m_InstanceOwned = true;
		m_pInstance = new ComponentTypes();
		return m_pInstance;
	}

	void ComponentTypes::DestroyInstance()
	{
		if (m_InstanceOwned) delete m_pInstance;
		m_pInstance = nullptr;
	}

	void ComponentTypes::SetInstance(ComponentTypes* pInstance)
	{
		m_pInstance = pInstance;
	}

	void ComponentTypes::AddTypeView(size_t hash, BaseTypeView* pTypeView)
	{
		if (m_pInstance->m_pTypeViewTemplates.find(hash) != m_pInstance->m_pTypeViewTemplates.end())
			throw new std::exception("Component already registered!");

		m_pInstance->m_pTypeViewTemplates.emplace(hash, pTypeView);
	}

	BaseTypeView* ComponentTypes::CreateTypeView(EntityRegistry* pRegistry, size_t hash)
	{
		if (m_pInstance->m_pTypeViewTemplates.find(hash) == m_pInstance->m_pTypeViewTemplates.end())
			throw new std::exception("Component not registered!");

		BaseTypeView* pTypeView = m_pInstance->m_pTypeViewTemplates[hash]->Create(pRegistry);
		pRegistry->m_pTypeViews.emplace(hash, pTypeView);
		return pTypeView;
	}

	ComponentTypes::ComponentTypes() : m_pTypeViewTemplates()
	{
	}
	
	ComponentTypes::~ComponentTypes()
	{
		for (auto it = m_pTypeViewTemplates.begin(); it != m_pTypeViewTemplates.end(); it++)
		{
			delete it->second;
		}
		m_pTypeViewTemplates.clear();
	}
}
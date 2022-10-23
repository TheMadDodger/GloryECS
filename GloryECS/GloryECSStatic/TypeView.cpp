#include "TypeView.h"

namespace GloryECS
{
	BaseTypeView::BaseTypeView(size_t typeHash) : m_TypeHash(typeHash), m_Entities()
	{
	}

	BaseTypeView::~BaseTypeView()
	{
		m_Entities.clear();
	}

	void BaseTypeView::Remove(EntityID entityID)
	{
		auto it = std::find(m_Entities.begin(), m_Entities.end(), entityID);
		if (it == m_Entities.end()) return;
		size_t index = it - m_Entities.begin();
		m_Entities.erase(it);
		OnRemove(index);
	}

	bool BaseTypeView::Contains(EntityID entityID)
	{
		return std::find(m_Entities.begin(), m_Entities.end(), entityID) != m_Entities.end();
	}
}

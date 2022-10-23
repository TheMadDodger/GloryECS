#include "EntityView.h"
#include "EntityRegistry.h"

namespace GloryECS
{
    EntityView::EntityView(EntityRegistry* pRegistry) : m_pRegistry(pRegistry)
    {
    }

    size_t EntityView::ComponentCount()
    {
        return m_ComponentTypes.size();
    }

    std::map<Glory::UUID, size_t>::iterator EntityView::GetIterator()
    {
        return m_ComponentTypes.begin();
    }

    std::map<Glory::UUID, size_t>::iterator EntityView::GetIteratorEnd()
    {
        return m_ComponentTypes.end();
    }

    size_t EntityView::ComponentTypeAt(size_t index)
    {
        return m_ComponentTypes[index];
    }

    void EntityView::Add(size_t hash, Glory::UUID uuid)
    {
        m_ComponentTypes.emplace(hash, uuid);
        m_TypeToUUID.emplace(uuid, hash);
    }

    void EntityView::Remove(size_t hash)
    {
        if (m_TypeToUUID.find(hash) == m_TypeToUUID.end()) return;
        Glory::UUID uuid = m_TypeToUUID[hash];
        m_TypeToUUID.erase(hash);
        m_ComponentTypes.erase(uuid);
    }

    void EntityView::Remove(Glory::UUID uuid)
    {
        if (m_ComponentTypes.find(uuid) == m_ComponentTypes.end()) return;
        size_t hash = m_ComponentTypes[uuid];
        m_ComponentTypes.erase(uuid);
        m_TypeToUUID.erase(hash);
    }
}

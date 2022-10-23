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

    size_t EntityView::ComponentTypeAt(size_t index)
    {
        return m_ComponentTypes[index];
    }
}

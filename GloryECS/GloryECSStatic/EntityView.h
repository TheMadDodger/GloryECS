#pragma once
#include <vector>

namespace GloryECS
{
	class EntityRegistry;

	class EntityView
	{
	public:
		EntityView(EntityRegistry* pRegistry);

		size_t ComponentCount();
		size_t ComponentTypeAt(size_t index);

	private:
		friend class EntityRegistry;
		EntityRegistry* m_pRegistry;
		std::vector<size_t> m_ComponentTypes;
	};
}

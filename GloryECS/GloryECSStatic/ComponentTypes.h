#pragma once
#include "TypeView.h"
#include <map>

namespace GloryECS
{
	class EntityRegistry;

	class ComponentTypes
	{
	public:
		template<typename T>
		static void RegisterComponent()
		{
			TypeView<T>* pTypeView = new TypeView<T>(nullptr);
			size_t hash = std::hash<std::type_index>()(typeid(T));
			AddTypeView(hash, pTypeView);
		}

		static ComponentTypes* CreateInstance();
		static void DestroyInstance();
		static void SetInstance(ComponentTypes* pInstance);

	private:
		static void AddTypeView(size_t hash, BaseTypeView* pTypeView);

		template<typename T>
		static TypeView<T>* CreateTypeView(EntityRegistry* pRegistry)
		{
			size_t hash = std::hash<std::type_index>()(typeid(T));
			return CreateTypeView(pRegistry, hash);
		}

		static BaseTypeView* CreateTypeView(EntityRegistry* pRegistry, size_t hash);

	private:
		ComponentTypes();
		virtual ~ComponentTypes();

	private:
		friend class EntityRegistry;
		static ComponentTypes* m_pInstance;
		static bool m_InstanceOwned;

		std::map<size_t, BaseTypeView*> m_pTypeViewTemplates;
	};
}

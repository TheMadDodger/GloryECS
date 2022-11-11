#pragma once
#include "TypeView.h"
#include <map>
#include <string>

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
			std::type_index type = typeid(T);
			size_t hash = std::hash<std::type_index>()(type);
			std::string name = type.name();
			AddTypeView(name, hash, pTypeView);
		}

		static ComponentTypes* CreateInstance();
		static void DestroyInstance();
		static void SetInstance(ComponentTypes* pInstance);
		static size_t GetComponentHash(const std::string& name);

	private:
		static void AddTypeView(std::string& name, size_t hash, BaseTypeView* pTypeView);

		template<typename T>
		static TypeView<T>* CreateTypeView(EntityRegistry* pRegistry)
		{
			size_t hash = std::hash<std::type_index>()(typeid(T));
			return CreateTypeView(pRegistry, hash);
		}

		static BaseTypeView* CreateTypeView(EntityRegistry* pRegistry, size_t hash);

		static void ProcessName(std::string& name);

	private:
		ComponentTypes();
		virtual ~ComponentTypes();

	private:
		friend class EntityRegistry;
		static ComponentTypes* m_pInstance;
		static bool m_InstanceOwned;

		std::map<size_t, BaseTypeView*> m_pTypeViewTemplates;
		std::map<std::string, size_t> m_NameToHash;
	};
}

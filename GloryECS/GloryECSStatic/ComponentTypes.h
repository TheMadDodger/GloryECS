#pragma once
#include "TypeView.h"
#include <map>
#include <string>

namespace GloryECS
{
	class EntityRegistry;

	struct ComponentType
	{
		ComponentType()
			: m_Name("INVALID"), m_TypeHash(0), m_AllowMultiple(false), m_CustomFags(0) {}
		ComponentType(const std::string& name, const size_t typeHash, const bool allowMultiple, const uint64_t customFlags)
			: m_Name(name), m_TypeHash(typeHash), m_AllowMultiple(allowMultiple), m_CustomFags(customFlags) {}

		const std::string m_Name;
		const size_t m_TypeHash;
		const bool m_AllowMultiple;
		const uint64_t m_CustomFags;
	};

	class ComponentTypes
	{
	public:
		template<typename T>
		static void RegisterComponent(bool allowMultiple = false, const uint64_t customFlags = 0)
		{
			TypeView<T>* pTypeView = new TypeView<T>(nullptr);
			std::type_index type = typeid(T);
			size_t hash = std::hash<std::type_index>()(type);
			m_pInstance->m_TypeHashes.push_back(hash);
			std::string name = type.name();
			AddTypeView(name, hash, pTypeView);
			AddComponentType(name, hash, allowMultiple, customFlags);
		}

		static ComponentTypes* CreateInstance();
		static void DestroyInstance();
		static void SetInstance(ComponentTypes* pInstance);
		static size_t GetComponentHash(const std::string& name);
		static const size_t ComponentCount();
		static const ComponentType* GetComponentType(const size_t hash);
		static const ComponentType* GetComponentTypeAt(const size_t index);

	private:
		static void AddTypeView(std::string& name, size_t hash, BaseTypeView* pTypeView);
		static void AddComponentType(std::string& name, size_t hash, bool allowMultiple, uint64_t customFlags);

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

		std::vector<size_t> m_TypeHashes;
		std::map<size_t, BaseTypeView*> m_pTypeViewTemplates;
		std::map<std::string, size_t> m_NameToHash;
		std::map<size_t, ComponentType> m_ComponentTypes;
	};
}

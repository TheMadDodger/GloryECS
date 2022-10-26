#pragma once
#include "ReflectGen.h"
#include "TypeData.h"
#include <map>

namespace GloryReflect
{
	class FactoryBase
	{
	public:
		FactoryBase();
		~FactoryBase();

	private:

	};

	class Reflect
	{
	public:
		template<typename T>
		static void RegisterType()
		{
			const TypeData* pTypeData = T::GetTypeData();
			RegisterType(pTypeData->TypeHash(), pTypeData);
		}

		static void RegisterType(size_t hash, const TypeData* pTypeData);

		static const TypeData* GetTyeData(size_t hash);

		static Reflect* CreateReflectInstance();
		static void DestroyReflectInstance();
		static void SetReflectInstance(Reflect* pInstance);

	private:
		Reflect();
		virtual ~Reflect();

	private:
		std::map<size_t, const TypeData*> m_pTypeDatas;

		static Reflect* m_pReflectInstance;
		static bool m_InstanceOwned;
	};
}
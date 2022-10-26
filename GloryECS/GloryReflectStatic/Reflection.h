#pragma once
#include "ReflectGen.h"
#include "TypeData.h"
#include "Factory.h"
#include <map>

namespace GloryReflect
{
	class Reflect
	{
	public:
		template<typename T>
		static const TypeData* RegisterType(uint64_t flags = 0)
		{
			const TypeData* pTypeData = T::GetTypeData();
			RegisterType(pTypeData->TypeHash(), pTypeData, flags);
			m_pReflectInstance->m_pFactories.emplace(pTypeData->TypeHash(), new Factory<T>());
			return pTypeData;
		}

		static void RegisterType(size_t hash, const TypeData* pTypeData, uint64_t flags = 0);

		static size_t TypeCount();
		static const TypeData* GetTyeData(size_t hash);
		static const TypeData* GetTyeDataAt(size_t index);
		static const uint64_t GetTypeFlags(size_t hash);
		static void SetFieldFlags(const FieldData* pFieldData, uint64_t flags = 0);
		static uint64_t GetFieldFlags(const FieldData* pFieldData);
		static std::map<size_t, const TypeData*>::iterator GetTypeIterator();
		static std::map<size_t, const TypeData*>::iterator GetTypeIteratorEnd();

		static std::any CreateAsValue(size_t hash);
		static void* CreateAsPointer(size_t hash);

		static Reflect* CreateReflectInstance();
		static void DestroyReflectInstance();
		static void SetReflectInstance(Reflect* pInstance);


	private:
		Reflect();
		virtual ~Reflect();

	private:
		std::map<size_t, const TypeData*> m_pTypeDatas;
		std::map<size_t, uint64_t> m_DataTypeFlags;
		std::map<const FieldData*, uint64_t> m_FieldFlags;
		std::map<size_t, const FactoryBase*> m_pFactories;

		static Reflect* m_pReflectInstance;
		static bool m_InstanceOwned;
	};
}
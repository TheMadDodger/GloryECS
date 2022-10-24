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

	private:
		Reflect();
		virtual ~Reflect();

	private:
		static std::map<size_t, const TypeData*> m_pTypeDatas;
	};


}
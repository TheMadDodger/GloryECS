#include "Reflection.h"

namespace GloryReflect
{
	std::map<size_t, const TypeData*> Reflect::m_pTypeDatas;

	void Reflect::RegisterType(size_t hash, const TypeData* pTypeData)
	{
		if (m_pTypeDatas.find(hash) != m_pTypeDatas.end()) return;
		m_pTypeDatas.emplace(hash, pTypeData);
	}

	const TypeData* Reflect::GetTyeData(size_t hash)
	{
		if (m_pTypeDatas.find(hash) == m_pTypeDatas.end()) return nullptr;
		return m_pTypeDatas[hash];
	}

	Reflect::Reflect()
	{
	}

	Reflect::~Reflect()
	{
	}
}
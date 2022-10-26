#include "Reflection.h"

namespace GloryReflect
{
	Reflect* Reflect::m_pReflectInstance = nullptr;
	bool Reflect::m_InstanceOwned = false;

	void Reflect::RegisterType(size_t hash, const TypeData* pTypeData)
	{
		if (m_pReflectInstance->m_pTypeDatas.find(hash) != m_pReflectInstance->m_pTypeDatas.end()) return;
		m_pReflectInstance->m_pTypeDatas.emplace(hash, pTypeData);
	}

	const TypeData* Reflect::GetTyeData(size_t hash)
	{
		if (m_pReflectInstance->m_pTypeDatas.find(hash) == m_pReflectInstance->m_pTypeDatas.end()) return nullptr;
		return m_pReflectInstance->m_pTypeDatas[hash];
	}

	std::any Reflect::CreateAsValue(size_t hash)
	{
		if (m_pReflectInstance->m_pFactories.find(hash) == m_pReflectInstance->m_pFactories.end())
			throw new std::exception("Type not found");

		const FactoryBase* pFactory = m_pReflectInstance->m_pFactories[hash];
		return pFactory->CreateAsValue();
	}

	void* Reflect::CreateAsPointer(size_t hash)
	{
		if (m_pReflectInstance->m_pFactories.find(hash) == m_pReflectInstance->m_pFactories.end())
			throw new std::exception("Type not found");

		const FactoryBase* pFactory = m_pReflectInstance->m_pFactories[hash];
		return pFactory->CreateAsPointer();
	}

	Reflect* Reflect::CreateReflectInstance()
	{
		m_InstanceOwned = true;
		m_pReflectInstance = new Reflect();
		return m_pReflectInstance;
	}

	void Reflect::DestroyReflectInstance()
	{
		if (m_InstanceOwned) delete m_pReflectInstance;
		m_pReflectInstance = nullptr;
	}

	void Reflect::SetReflectInstance(Reflect* pInstance)
	{
		m_pReflectInstance = pInstance;
	}

	Reflect::Reflect()
	{
	}

	Reflect::~Reflect()
	{
		for (auto it = m_pFactories.begin(); it != m_pFactories.end(); it++)
		{
			delete it->second;
		}

		m_pTypeDatas.clear();
		m_pFactories.clear();
	}
}
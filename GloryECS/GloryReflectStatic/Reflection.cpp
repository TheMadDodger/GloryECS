#include "Reflection.h"

namespace GloryReflect
{
	Reflect* Reflect::m_pReflectInstance = nullptr;
	bool Reflect::m_InstanceOwned = false;
	const char* Reflect::BASIC_VALUE_NAME = "m_value";

	void Reflect::RegisterType(size_t hash, const TypeData* pTypeData, uint64_t flags)
	{
		if (m_pReflectInstance->m_pTypeDatas.find(hash) != m_pReflectInstance->m_pTypeDatas.end()) return;
		m_pReflectInstance->m_pTypeDatas.emplace(hash, pTypeData);
		m_pReflectInstance->m_DataTypeFlags.emplace(hash, flags);
		m_pReflectInstance->m_StringToTypeHash.emplace(pTypeData->TypeName(), hash);
	}

	const TypeData* Reflect::RegisterBasicType(const std::type_info& type, size_t size, const std::string& aliasName, uint64_t flags)
	{
		const size_t typeHash = std::hash<std::type_index>()(type);
		if (m_pReflectInstance->m_pTypeDatas.find(typeHash) != m_pReflectInstance->m_pTypeDatas.end()) return m_pReflectInstance->m_pTypeDatas[typeHash];

		const char* typeNameString = type.name();
		const FieldData* pFields = new FieldData(typeHash, typeHash, BASIC_VALUE_NAME, typeNameString, 0, size);
		const TypeData* pTypeData = new TypeData(typeNameString, pFields, typeHash, 1, true);

		RegisterType(typeHash, pTypeData, flags);
		m_pReflectInstance->m_pManagedTypeDatas.push_back(pTypeData);
		if (aliasName != "") m_pReflectInstance->m_StringToTypeHash.emplace(aliasName, typeHash);
		return pTypeData;
	}

	const TypeData* Reflect::RegisterEnumType(const char* typeName, size_t enumTypeHash, const std::string& aliasName, uint64_t flags)
	{
		const TypeData* pTypeData = new TypeData(typeName, enumTypeHash);
		RegisterType(enumTypeHash, pTypeData, flags);
		m_pReflectInstance->m_pManagedTypeDatas.push_back(pTypeData);
		if (aliasName != "") m_pReflectInstance->m_StringToTypeHash.emplace(aliasName, enumTypeHash);
		return pTypeData;
	}

	void Reflect::Tokenize(std::string str, std::vector<std::string>& tokens, char separator)
	{
		size_t pos = 0;
		while ((pos = str.find(separator)) != std::string::npos)
		{
			std::string token = str.substr(0, pos);
			tokens.push_back(token);
			str.erase(0, pos + 1);
		}

		if (str.length() > 0 && tokens.size() <= 0) tokens.push_back(str);
	}

	const TypeData* Reflect::RegisterTemplatedType(const char* typeName, size_t typeHash, size_t size)
	{
		if (m_pReflectInstance->m_pTypeDatas.find(typeHash) != m_pReflectInstance->m_pTypeDatas.end()) return m_pReflectInstance->m_pTypeDatas[typeHash];
		std::string tempTypeName = typeName;

		std::vector<std::string> tokens;
		Tokenize(typeName, tokens);

		const char* typeNameString = tokens[0].c_str();
		const size_t TYPE_HASH = typeHash;
		const int NUM_ARGS = 0;
		const FieldData pFields[] =
		{
			FieldData(TYPE_HASH, BASIC_VALUE_NAME, typeNameString, 0, size)
		};
		const TypeData* pTypeData = new TypeData(typeName, pFields, TYPE_HASH, NUM_ARGS);

		m_pReflectInstance->m_pTypeDatas.emplace(TYPE_HASH, pTypeData);
		m_pReflectInstance->m_DataTypeFlags.emplace(TYPE_HASH, 0);

		for (size_t i = 0; i < tokens.size(); i++)
		{
			m_pReflectInstance->m_StringToTypeHash.emplace(tokens[i], typeHash);
		}

		m_pReflectInstance->m_pManagedTypeDatas.push_back(pTypeData);

		return pTypeData;
	}

	size_t Reflect::TypeCount()
	{
		return m_pReflectInstance->m_pTypeDatas.size();
	}

	const TypeData* Reflect::GetTyeData(size_t hash)
	{
		if (m_pReflectInstance->m_pTypeDatas.find(hash) == m_pReflectInstance->m_pTypeDatas.end()) return nullptr;
		return m_pReflectInstance->m_pTypeDatas[hash];
	}

	const TypeData* Reflect::GetTyeData(const std::string& name)
	{
		if (m_pReflectInstance->m_StringToTypeHash.find(name) == m_pReflectInstance->m_StringToTypeHash.end()) return nullptr;
		size_t typeHash = m_pReflectInstance->m_StringToTypeHash[name];
		return GetTyeData(typeHash);
	}

	const TypeData* Reflect::GetTyeDataAt(size_t index)
	{
		auto it = m_pReflectInstance->m_pTypeDatas.begin();
		std::advance(it, index);
		return it->second;
	}

	const uint64_t Reflect::GetTypeFlags(size_t hash)
	{
		if (m_pReflectInstance->m_DataTypeFlags.find(hash) == m_pReflectInstance->m_DataTypeFlags.end()) return 0;
		return m_pReflectInstance->m_DataTypeFlags[hash];
	}

	void Reflect::SetFieldFlags(const FieldData* pFieldData, uint64_t flags)
	{
		m_pReflectInstance->m_FieldFlags[pFieldData] = flags;
	}

	uint64_t Reflect::GetFieldFlags(const FieldData* pFieldData)
	{
		if (m_pReflectInstance->m_FieldFlags.find(pFieldData) == m_pReflectInstance->m_FieldFlags.end()) return 0;
		return m_pReflectInstance->m_FieldFlags[pFieldData];
	}

	std::map<size_t, const TypeData*>::iterator Reflect::GetTypeIterator()
	{
		return m_pReflectInstance->m_pTypeDatas.begin();
	}

	std::map<size_t, const TypeData*>::iterator Reflect::GetTypeIteratorEnd()
	{
		return m_pReflectInstance->m_pTypeDatas.end();
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

	void Reflect::CreateAsTemporary(size_t hash, std::function<void(void*)> callback)
	{
		if (m_pReflectInstance->m_pFactories.find(hash) == m_pReflectInstance->m_pFactories.end())
			throw new std::exception("Type not found");

		const FactoryBase* pFactory = m_pReflectInstance->m_pFactories[hash];
		pFactory->CreateAsTemporary(callback);
	}

	Reflect* Reflect::CreateReflectInstance()
	{
		m_InstanceOwned = true;
		m_pReflectInstance = new Reflect();

		RegisterBasicType<int8_t>();
		RegisterBasicType<int16_t>();
		RegisterBasicType<int32_t>();
		RegisterBasicType<int64_t>();
		RegisterBasicType<uint8_t>();
		RegisterBasicType<uint16_t>();
		RegisterBasicType<uint32_t>();
		RegisterBasicType<uint64_t>();
		RegisterBasicType<char>();
		RegisterBasicType<bool>();
		RegisterBasicType<float>();
		RegisterBasicType<double>();
		RegisterBasicType<long>();
		RegisterBasicType<unsigned long>();

		RegisterTemplatedType("std::vector,vector", (size_t)CustomTypeHash::Array, 0);

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

	void Reflect::ResizeArray(void* pArrayAddress, size_t elementTypeHash, size_t newSize)
	{
		if (m_pReflectInstance->m_pArrayTypes.find(elementTypeHash) == m_pReflectInstance->m_pArrayTypes.end()) return;
		m_pReflectInstance->m_pArrayTypes[elementTypeHash]->Resize(pArrayAddress, newSize);
	}

	const size_t Reflect::ArraySize(void* pArrayAddress, size_t elementTypeHash)
	{
		if (m_pReflectInstance->m_pArrayTypes.find(elementTypeHash) == m_pReflectInstance->m_pArrayTypes.end()) return 0;
		return m_pReflectInstance->m_pArrayTypes[elementTypeHash]->Size(pArrayAddress);
	}

	void* Reflect::ElementAddress(void* pArrayAddress, size_t elementTypeHash, size_t index)
	{
		if (m_pReflectInstance->m_pArrayTypes.find(elementTypeHash) == m_pReflectInstance->m_pArrayTypes.end()) return 0;
		return m_pReflectInstance->m_pArrayTypes[elementTypeHash]->ElementAddress(pArrayAddress, index);
	}

	EnumType* Reflect::GetEnumType(size_t hash)
	{
		if (m_pReflectInstance->m_pEnumTypes.find(hash) == m_pReflectInstance->m_pEnumTypes.end()) return nullptr;
		return m_pReflectInstance->m_pEnumTypes[hash];
	}

	size_t Reflect::GetCustomTypeHash(size_t hash)
	{
		if (hash <= 100) return hash;
		if (GetEnumType(hash)) return (size_t)CustomTypeHash::Enum;
		const TypeData* pTypeData = GetTyeData(hash);
		if (pTypeData && !pTypeData->IsBasicType()) return (size_t)CustomTypeHash::Struct;
		return hash;
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
		
		for (auto it = m_pArrayTypes.begin(); it != m_pArrayTypes.end(); it++)
		{
			delete it->second;
		}
		
		for (auto it = m_pEnumTypes.begin(); it != m_pEnumTypes.end(); it++)
		{
			delete it->second;
		}

		for (size_t i = 0; i < m_pManagedTypeDatas.size(); i++)
		{
			for (int j = 0; j < m_pManagedTypeDatas[i]->FieldCount(); j++)
			{
				const FieldData* pFieldData = m_pManagedTypeDatas[i]->GetFieldData(j);
				delete pFieldData;
			}
			delete m_pManagedTypeDatas[i];
		}

		m_pTypeDatas.clear();
		m_pManagedTypeDatas.clear();
		m_StringToTypeHash.clear();
		m_DataTypeFlags.clear();
		m_FieldFlags.clear();
		m_pFactories.clear();
		m_pArrayTypes.clear();
		m_pEnumTypes.clear();
	}
}
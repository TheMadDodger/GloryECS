#include "TypeData.h"
#include "CustomTypeHash.h"

namespace GloryReflect
{
	TypeData::TypeData(const char* typeName, const FieldData* pFields, uint32_t typeHash, int numFields, bool isBasicType) :
		m_TypeName(typeName),
		m_pFields(pFields),
		m_TypeHash(typeHash),
		m_FieldCount(numFields),
		m_BasicType(isBasicType)
	{
	}

	TypeData::TypeData(const char* typeName, uint32_t enumTypeHash) :
		m_TypeName(typeName),
		m_pFields(new FieldData((size_t)CustomTypeHash::Enum, enumTypeHash, "m_value", typeName, 0, sizeof(size_t))),
		m_TypeHash(enumTypeHash),
		m_FieldCount(1),
		m_BasicType(false)
	{
	}

	TypeData::~TypeData()
	{
	}

	const char* TypeData::TypeName() const
	{
		return m_TypeName;
	}

	const uint32_t TypeData::TypeHash() const
	{
		return m_TypeHash;
	}

	const size_t TypeData::FieldCount() const
	{
		return m_FieldCount;
	}

	const bool TypeData::IsBasicType() const
	{
		return m_BasicType;
	}

	const FieldData* TypeData::GetFieldData(size_t index) const
	{
		return &m_pFields[index];
	}
}

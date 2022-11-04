#include "TypeData.h"
#include "CustomTypeHash.h"

namespace GloryReflect
{
	TypeData::TypeData(const char* typeName, const FieldData* pFields, size_t typeHash, int numFields) :
		m_TypeName(typeName),
		m_pFields(pFields),
		m_TypeHash(typeHash),
		m_FieldCount(numFields)
	{
	}

	TypeData::TypeData(const char* typeName, size_t enumTypeHash) :
		m_TypeName(typeName),
		m_pFields(new FieldData((size_t)CustomTypeHash::Enum, enumTypeHash, "m_value", typeName, 0, sizeof(size_t))),
		m_TypeHash(enumTypeHash),
		m_FieldCount(1)
	{
	}

	TypeData::~TypeData()
	{
	}

	const char* TypeData::TypeName() const
	{
		return m_TypeName;
	}

	const size_t TypeData::TypeHash() const
	{
		return m_TypeHash;
	}

	const int TypeData::FieldCount() const
	{
		return m_FieldCount;
	}

	const FieldData* TypeData::GetFieldData(size_t index) const
	{
		return &m_pFields[index];
	}
}

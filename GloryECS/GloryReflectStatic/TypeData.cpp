#include "TypeData.h"

namespace GloryReflect
{
	TypeData::TypeData(const FieldData* pFields, size_t typeHash, int numFields) : m_pFields(pFields), m_TypeHash(typeHash), m_FieldCount(numFields)
	{
	}

	TypeData::~TypeData()
	{
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

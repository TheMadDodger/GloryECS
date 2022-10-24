#pragma once
#include "FieldData.h"

namespace GloryReflect
{
	struct TypeData
	{
	public:
		TypeData(const FieldData* pFields, size_t typeHash, int numFields);
		virtual ~TypeData();

		const size_t TypeHash() const;
		const int FieldCount() const;
		const FieldData* GetFieldData(size_t index) const;

	private:
		const size_t m_TypeHash;
		const int m_FieldCount;
		const FieldData* m_pFields;
	};
}
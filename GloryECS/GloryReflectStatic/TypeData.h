#pragma once
#include "FieldData.h"

namespace GloryReflect
{
	struct TypeData
	{
	public:
		TypeData(const char* typeName, const FieldData* pFields, size_t typeHash, int numFields);
		TypeData(const char* typeName, size_t enumTypeHash);
		virtual ~TypeData();

		const char* TypeName() const;
		const size_t TypeHash() const;
		const int FieldCount() const;
		const FieldData* GetFieldData(size_t index) const;

	private:
		const char* m_TypeName;
		const size_t m_TypeHash;
		const int m_FieldCount;
		const FieldData* m_pFields;
	};
}
#include "FieldData.h"
#include <string>

namespace GloryReflect
{
	FieldData::FieldData(FieldType type, const char* name, const char* typeName, size_t offset, size_t size) :
		m_Type(type),
		m_Name(name),
		m_TypeName(typeName),
		m_Offset(offset),
		m_Size(size)
	{
	}

	FieldData::~FieldData()
	{
	}

	const FieldType FieldData::Type() const
	{
		return m_Type;
	}

	const char* FieldData::Name() const
	{
		return m_Name;
	}

	const char* FieldData::TypeName() const
	{
		return m_TypeName;
	}

	const size_t FieldData::Offset() const
	{
		return m_Offset;
	}

	const size_t FieldData::Size() const
	{
		return m_Size;
	}

	void FieldData::Get(void* pAddress, void* out) const
	{
		char* p = (char*)pAddress;
		memcpy(out, p + m_Offset, m_Size);
	}

	void FieldData::Set(void* pAddress, void* value) const
	{
		char* p = (char*)pAddress;
		memcpy(p + m_Offset, value, m_Size);
	}
}

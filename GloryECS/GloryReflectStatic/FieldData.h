#pragma once

namespace GloryReflect
{
	using FieldType = size_t;

	enum StorageType
	{
		ST_Value,
		ST_Array,
		ST_Struct,
	};

	struct FieldData
	{
	public:
		FieldData(FieldType type, const char* name, const char* typeName, size_t offset, size_t size);
		virtual ~FieldData();

		const FieldType Type() const;
		const char* Name() const;
		const char* TypeName() const;
		const size_t Offset() const;
		const size_t Size() const;

		void Get(void* pAddress, void* out) const;
		void Set(void* pAddress, void* value) const;

	private:
		StorageType m_StorageType;
		FieldType m_Type;
		const char* m_Name;
		const char* m_TypeName;
		size_t m_Offset;
		size_t m_Size;
	};
}

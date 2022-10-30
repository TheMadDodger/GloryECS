#pragma once
#include <cstddef>
#include "MacroHelpers.h"
#include "TypeData.h"
#include <typeindex>

#define REFLECTABLE_FIELD(...)\
ARGPAIR(__VA_ARGS__);

#define REFLECT_FIELD_INFO(x)\
GloryReflect::FieldData(std::hash<std::type_index>()(typeid(ARGNAME(x))), ARGNAME_AS_STRING(x), ARGTYPE_AS_STRING(x), offsetof(TypeName, ARGNAME(x)), sizeof(ARGTYPE(x))),

#define REFLECTABLE(typeName, ...)\
FOR_EACH(REFLECTABLE_FIELD, __VA_ARGS__)\
typedef typeName TypeName;\
public:\
	static const GloryReflect::TypeData* GetTypeData()\
	{\
		static const char* typeNameString = STRINGIZE(typeName);\
		static const size_t TYPE_HASH = std::hash<std::type_index>()(typeid(typeName));\
		static const int NUM_ARGS = NARGS(__VA_ARGS__);\
		static const GloryReflect::FieldData pFields[] = {\
			FOR_EACH(REFLECT_FIELD_INFO, __VA_ARGS__)\
		};\
		static const GloryReflect::TypeData pTypeData = GloryReflect::TypeData(typeNameString, pFields, TYPE_HASH, NUM_ARGS);\
		return &pTypeData;\
	}


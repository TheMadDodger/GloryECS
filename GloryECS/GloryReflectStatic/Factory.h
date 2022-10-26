#pragma once
#include <any>
#include <string>
#include <typeindex>

namespace GloryReflect
{
	class FactoryBase
	{
	public:
		FactoryBase(size_t typeHash);
		virtual ~FactoryBase();

		virtual std::any CreateAsValue() const = 0;
		virtual void* CreateAsPointer() const = 0;

	private:
		friend class Reflect;
		const size_t m_TypeHash;
	};

	template<typename T>
	class Factory : public FactoryBase
	{
	public:
		Factory() : FactoryBase(std::hash<std::type_index>()(typeid(T))) {}
		virtual ~Factory() {}

		virtual std::any CreateAsValue() const override
		{
			return T();
		}

		virtual void* CreateAsPointer() const override
		{
			return new T();
		}
	};
}

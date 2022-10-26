#include "Factory.h"

namespace GloryReflect
{
	FactoryBase::FactoryBase(size_t typeHash) : m_TypeHash(typeHash)
	{
	}
	FactoryBase::~FactoryBase()
	{
	}
}

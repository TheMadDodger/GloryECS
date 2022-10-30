#pragma once
#include <vector>

namespace GloryReflect
{
	class ArrayTypeBase
	{
	public:
		virtual void Resize(void* pArrayAddress, size_t newSize) const = 0;
	};

	template<typename T>
	class ArrayType : public ArrayTypeBase
	{
	public:
		virtual void Resize(void* pArrayAddress, size_t newSize) const override
		{
			ResizeInternal((std::vector<T>*)pArrayAddress, newSize);
		}

	private:
		void ResizeInternal(std::vector<T>* pArray, size_t newSize) const
		{
			size_t currentSize = pArray->size();
			if (currentSize > 0)
			{
				T value = pArray->at(currentSize - 1);
				pArray->resize(newSize, value);
				return;
			}

			pArray->resize(newSize);
		}
	};
}

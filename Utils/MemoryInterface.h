#ifndef __MEM_INTERFACE_DA_H___
#define __MEM_INTERFACE_DA_H___

#include "UtilsClient.h"
#include <xmmintrin.h>


namespace DifferentialArts
{
	/*!	\class BaseMemoryInterface
		\brief Memory interface class for classes that contain SSE objects
		\author Rajesh Peter Douglas D'Monte	

		This class is used to override a classes C++ memory alloc/deallocation operators
		to 16 byte aligned alloc/deallocation operators
	*/
	class RX_UTIL_API BaseMemoryInterface
	{
	public:
		void* operator new (size_t size)
		{
			return _mm_malloc(size, 16);
		}
		//!< new C++ operator

		void* operator new[] (size_t size)
		{
			return _mm_malloc(size, 16);
		}

		//!< new[] C++ operator

		void operator delete (void *mem)
		{
			_mm_free(mem);
		}

		//!< delete C++ operator

		void operator delete[] (void *mem)
		{
			_mm_free(mem);
		}

		//!< delete[] C++ operator
	};
}

#endif
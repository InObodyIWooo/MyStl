#pragma once


#ifndef __STL_ALLOC_H
#define __STL_ALLOC_H

#if !defined(__THROW_ZERO_POINTER)
#include <iostream>
#define __THROW_ZERO_POINTER std::cerr << "pointer error" << std::endl; exit(0);
#endif

#include "__default_alloc_template.h"

namespace mystl
{
	template<class T,class Alloc>
	class MyAlloc
	{
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;;
		typedef const T& const_reference;
		typedef ptrdiff_t difference_type;
	public :

		static pointer allocate();
		static pointer allocate(size_t);
		static void deallocate(pointer);
		static void deallocate(pointer, size_t);

	};

	template<class T,class Alloc>
	typename MyAlloc<T, Alloc>::pointer MyAlloc<T, Alloc>::allocate()
	{
		return (pointer)Alloc::allocate(sizeof value_type);
	}

	template<class T, class Alloc>
	typename MyAlloc<T, Alloc>::pointer MyAlloc<T, Alloc>::allocate(size_t n)
	{
		return 0 == n ? nullptr : (pointer)Alloc::allocate(n * (sizeof value_type));
	}

	template<class T, class Alloc>
	void MyAlloc<T, Alloc>::deallocate(pointer p)
	{
		if (p == nullptr) { __THROW_ZERO_POINTER }
		Alloc::deallocate(p, sizeof value_type);
	}

	template<class T, class Alloc>
	void MyAlloc<T, Alloc>::deallocate(pointer p, size_t n)
	{
		if (p == nullptr) { __THROW_ZERO_POINTER }
		if (n != 0)Alloc::deallocate(p, n * (sizeof value_type));
	}


}
#endif // !__STL_ALLOC_H

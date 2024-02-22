#pragma once


#ifndef __STL_CONSTRUCT_H
#define __STL_CONSTRUCT_H

#include <new>


namespace mystl
{
	template<class T>
	inline void construct(T* p) {
		new((void*)p) T();
	}

	template<class T1, class T2>
	inline void construct(T1* p, const T2& value) {
		new((void*)p) T1(value);
	}

	template<class T, class... Args>
	inline void construct(T* p, Args&&... args) {
		//TODO: forward
		//new((void*)p) T1(value);
	}


	template<class T>
	inline void _destroy_one(T* pointer, std::false_type) {
		if (pointer != nullptr)pointer->~T();
	}

	template<class T>
	inline void _destroy_one(T* p, std::true_type) {}


	template<class T>
	inline void destroy(T* p) {
		typedef typename std::is_trivially_destructible<T> trivial_destructor;
		_destroy_one(p, trivial_destructor());
	}

	template<class ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, std::false_type) {
		for (; first != last; ++first)
			destroy(&*first);
	}

	template<class ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, std::true_type) {}

	template<class ForwardIterator, class T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T*) {
		typedef typename std::is_trivially_destructible<T> trivial_destructor;
		_destroy_aux(first, last, trivial_destructor());
	}

	template<class ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last) {
		_destroy(first, last, value_type(first));
	}

}

#endif
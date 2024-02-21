#pragma once

#ifndef __STL_UNINITIALIZED_H
#define __STL_UNINITIALIZED_H

#include "stl_iterator.h"
#include "../stl_allocator/stl_construct.h"



namespace mystl
{

	// uninitialized_fill_n

	template<class ForwardIterator, class Size, class T>
	inline  ForwardIterator
		uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
		return _uninitialized_fill_n(first, n, x, value_type(first));
	}

	template<class ForwardIterator, class Size, class T, class T1>
	inline  ForwardIterator
		_uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
		typedef typename std::is_pod<T1> is_Pod;
		return _uninitialized_fill_n_aux(first, n, x, is_Pod());
	}

	template<class ForwardIterator, class Size, class T>
	inline  ForwardIterator
		_uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, std::true_type) {
		//TODO: fill_n()
	}

	template<class ForwardIterator, class Size, class T>
	inline  ForwardIterator
		_uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, std::false_type) {
		ForwardIterator current = first;
		for (; n > 0; --n, ++current)
			constructor(&*current, x);
		return current;
	}


	// uninitialized_fill

	template<class ForwardIterator, class T>
	inline  void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
		return _uninitialized_fill(first, last, x, value_type(first));
	}

	template<class ForwardIterator, class T, class T1>
	inline  void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) {
		typedef typename std::is_pod<T1> is_Pod;
		return _uninitialized_fill_n_aux(first, last, x, is_Pod());
	}

	template<class ForwardIterator, class T>
	inline  void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, std::true_type) {
		//TODO: fill()
	}

	template<class ForwardIterator, class T>
	inline  void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, std::false_type) {
		ForwardIterator current = first;
		for (; current != last; ++current)
			constructor(&*current, x);
	}


	//uninitialized_copy

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		return _uninitialized_copy(first, last, result, value_type(result));
	}

	template<class InputIterator, class ForwardIterator, class T1>
	inline ForwardIterator
		_uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T1*) {
		typedef typename std::is_pod<T1> is_Pod;
		return _uninitialized_copy_aux(first, last, result, is_Pod());
	}

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::true_type) {
		//TODO: copy();
	}

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::false_type) {
		ForwardIterator current = result;
		for (; first != last; ++first, ++current)
			constructor(&*current, *first);
		return current;
	}
}

#endif // !__STL_UNINITIALIZED_H

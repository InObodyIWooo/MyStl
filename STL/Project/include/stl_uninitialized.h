#pragma once

#ifndef __STL_UNINITIALIZED_H
#define __STL_UNINITIALIZED_H

#include "stl_iterator.h"
#include "stl_construct.h"



namespace mystl
{
	// uninitialized_fill_n

	template<class ForwardIterator, class Size, class T>
	inline  ForwardIterator
		_uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, std::true_type) {
		//TODO: fill_n()
		return std::fill_n(first, n, x);
	}

	template<class ForwardIterator, class Size, class T>
	inline  ForwardIterator
		_uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, std::false_type) {
		ForwardIterator current = first;
		for (; n > 0; --n, ++current)
			construct(&*current, x);
		return current;
	}

	template<class ForwardIterator, class Size, class T, class T1>
	inline  ForwardIterator
		_uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*) {
		typedef typename std::is_pod<T1> is_Pod;
		return _uninitialized_fill_n_aux(first, n, x, is_Pod());
	}

	template<class ForwardIterator, class Size, class T>
	inline  ForwardIterator
		uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
		return _uninitialized_fill_n(first, n, x, value_type(first));
	}

	// uninitialized_fill

	template<class ForwardIterator, class T>
	inline  void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, std::true_type) {
		//TODO: fill()
		return std::fill(first, last, x);
	}

	template<class ForwardIterator, class T>
	inline  void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, std::false_type) {
		ForwardIterator current = first;
		for (; current != last; ++current)
			construct(&*current, x);
	}

	template<class ForwardIterator, class T, class T1>
	inline  void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*) {
		typedef typename std::is_pod<T1> is_Pod;
		return _uninitialized_fill_aux(first, last, x, is_Pod());
	}

	template<class ForwardIterator, class T>
	inline  void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x) {
		return _uninitialized_fill(first, last, x, value_type(first));
	}

	//uninitialized_copy

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::true_type) {
		//TODO: copy();
		return std::copy(first, last, result);
	}

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		_uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::false_type) {
		ForwardIterator current = result;
		for (; first != last; ++first, ++current)
			construct(&*current, *first);
		return current;
	}

	template<class InputIterator, class ForwardIterator, class T1>
	inline ForwardIterator
		_uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T1*) {
		typedef typename std::is_pod<T1> is_Pod;
		return _uninitialized_copy_aux(first, last, result, is_Pod());
	}


	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
		return _uninitialized_copy(first, last, result, value_type(result));
	}

	//uninitialized_copy_n

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		_uninitialized_copy_n_aux(InputIterator first, ptrdiff_t n, ForwardIterator result, std::true_type) {
		//TODO: copy_n();
		return std::copy_n(first, n, result);
	}

	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		_uninitialized_copy_n_aux(InputIterator first, ptrdiff_t n, ForwardIterator result, std::false_type) {
		ForwardIterator current = result;
		for (; n > 0;--n, ++first, ++current)
			construct(&*current, *first);
		return current;
	}

	template<class InputIterator, class ForwardIterator, class T1>
	inline ForwardIterator
		_uninitialized_copy_n(InputIterator first, ptrdiff_t n, ForwardIterator result, T1*) {
		typedef typename std::is_pod<T1> is_Pod;
		return _uninitialized_copy_n_aux(first, n, result, is_Pod());
	}


	template<class InputIterator, class ForwardIterator>
	inline ForwardIterator
		uninitialized_copy_n(InputIterator first, ptrdiff_t n, ForwardIterator result) {
		return _uninitialized_copy_n(first, n, result, value_type(result));
	}

}

#endif // !__STL_UNINITIALIZED_H

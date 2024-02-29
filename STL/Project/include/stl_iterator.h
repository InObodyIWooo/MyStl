#pragma once


#ifndef __STL_ITERATOR_TRAITS_H
#define __STL_ITERATOR_TRAITS_H

#include "type_traits.h"

namespace mystl
{
	//pointer diff
	//typedef long long Ptrdiff_t;

	//iterator category
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirection_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirection_iterator_tag {};

	template<class T,class Category, class Pointer = T*, class Reference = T&, class Distance = ptrdiff_t>
	struct iterator
	{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;//pointer diff
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	template<typename T>
	struct iterator_traits
	{
		typedef typename T::iterator_category iterator_category;
		typedef typename T::value_type value_type;
		typedef typename T::difference_type difference_type;
		typedef typename T::pointer pointer;
		typedef typename T::reference reference;
	};

	template<typename T>
	struct iterator_traits<T*>
	{
		typedef typename random_access_iterator_tag iterator_category;
		typedef typename T value_type;
		typedef typename ptrdiff_t difference_type;
		typedef typename T* pointer;
		typedef typename T& reference;
	};

	template<typename T>
	struct iterator_traits<const T*>
	{
		typedef typename random_access_iterator_tag iterator_category;
		typedef typename T value_type;
		typedef typename ptrdiff_t difference_type;
		typedef typename const T* pointer;
		typedef typename const T& reference;
	};

	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category
		iterator_category(const Iterator&) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category();
	}

	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type*
		value_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type*
		distance_type(const Iterator&) {
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}

	//distance

	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		_distance(InputIterator first, InputIterator last, input_iterator_tag) {
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last) {
			++first; ++n;
		}
		return n;
	}

	template<class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type
		_distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
		return last - first;
	}

	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type
		distance(InputIterator first, InputIterator last) {
		return _distance(first,last,iterator_category(InputIterator));
	}

	//advance

	template<class InputIterator,class Distance>
	inline void _advance(InputIterator& p,Distance n,input_iterator_tag) {
		while (n--)++p;
	}

	template<class BidiectionIterator, class Distance>
	inline void _advance(BidiectionIterator& p, Distance n, bidirection_iterator_tag) {
		if (n >= 0)
			while (n--)++p;
		else
			while (n--)--p;
	}
	template<class RandomAccessIterator, class Distance>
	inline void _advance(RandomAccessIterator& p, Distance n, random_access_iterator_tag) {
		p += n;
	}
	template<class InputIterator, class Distance>
	inline void advance(InputIterator& p, Distance n) {
		_advance(p, n, iterator_category(InputIterator));
	}
}

#endif

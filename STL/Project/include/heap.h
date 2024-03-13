#pragma once

#ifndef __STL_HEAP_H
#define __STL_HEAP_H

#include "stl_iterator.h"


namespace mystl
{
	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void _push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare& cmp);
	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void _push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value, Compare& cmp);

	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void _pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare& cmp);
	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void _pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*, Compare& cmp);
	template<class RandomAccessIterator, class Distance, class T, class Compare>
	void _adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value, Compare& cmp);

	template<class RandomAccessIterator, class Distance, class T, class Compare>
	void _make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare& cmp);

	template<class RandomAccessIterator,class Compare>
	inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
		_push_heap_aux(first, last, distance_type(first), value_type(first),cmp);
	}

	template<class RandomAccessIterator,class Distance,class T, class Compare>
	inline void _push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare& cmp) {
		_push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)), cmp);
	}

	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void _push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value, Compare& cmp) {
		Distance parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex && cmp(*(first + parent), value))
		{
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

	template<class RandomAccessIterator, class Compare>
	inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
		_pop_heap_aux(first, last, distance_type(first), value_type(first), cmp);
	}

	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void _pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare& cmp) {
		_pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first), cmp);
	}

	template<class RandomAccessIterator, class Distance, class T, class Compare>
	inline void _pop_heap(	RandomAccessIterator first, 
							RandomAccessIterator last, 
							RandomAccessIterator result, T value,Distance *, Compare& cmp) {
		*result = *first;
		_adjust_heap(first, Distance(0), Distance(last - first), value, cmp);
	}

	template<class RandomAccessIterator,class Distance,class T,class Compare>
	void _adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value, Compare& cmp) {
		Distance topIndex = holeIndex;
		Distance secondIndex = 2 * (holeIndex + 1);
		while (secondIndex < len)
		{
			if (cmp(*(first + secondIndex), *(first + secondIndex - 1)))
				--secondIndex;
			*(first + holeIndex) = *(first + secondIndex);
			holeIndex = secondIndex;
			secondIndex = 2 * (holeIndex + 1);
		}
		if (secondIndex == len) {
			*(first + holeIndex) = *(first + secondIndex - 1);
			holeIndex = secondIndex - 1;
		}
		_push_heap(first, holeIndex, topIndex, value, cmp);
	}

	template<class RandomAccessIterator,class Compare>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
		while (last - first > 1) pop_heap(first, last--, cmp);
	}

	template<class RandomAccessIterator,class Compare>
	inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare cmp) {
		_make_heap(first, last, distance_type(first), value_type(first),cmp);
	}

	template<class RandomAccessIterator, class Distance,class T, class Compare>
	void _make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare& cmp) {
		Distance len = last - first;
		Distance holeIndex = (len - 2) / 2;
		while (true)
		{
			_adjust_heap(first, holeIndex, len, *(first + holeIndex), cmp);
			if (holeIndex == 0)return;
			holeIndex--;
		}
	}
}
#endif
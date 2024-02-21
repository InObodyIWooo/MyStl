#pragma once


#ifndef __STL_VECTOR_H
#define __STL_VECTOR_H

#include "stl_alloc.h"

namespace mystl
{

	template<class T,class Alloc = _alloc>
	class vector
	{
	public:
		typedef T					value_type;
		typedef value_type*			pointer;
		typedef const value_type*	const_iterator;
		typedef value_type*			iterator;
		typedef value_type&			reference;
		typedef const value_type&	const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
	protected:
		typedef m_alloc<value_type, Alloc> data_alloc;

		iterator start;		//已使用空间的头指针
		iterator finish;	//已使用空间的尾指针
		iterator end_heap;		//未使用空间的尾指针

		void insert_aux(iterator position,const_reference value);

		void dellocate() {
			if (start != nullptr)data_alloc::deallocate(start, size_type(end_heap - start));
		}

		iterator allocate_fill(size_type n, const_reference value) {
			iterator res = data_alloc::allocate(n);
			uninitialized_fill_n(res, n, value);
			return res;
		}

		void initialize_fill(size_type n, const_reference value) {
			start = allocate_fill(n, value);
			end_heap = finish = start + n;
		}

	public:
		const_iterator begin() const { return start; }
		const_iterator end() const { return finish; }
		size_type size() const { return size_type(finish - start); }
		size_type capacity() const { return size_type(end - start); }
		bool empty() const { return finish == start; }
		reference operator[](size_type n) { return *(start + n); }

	public:
		vector() :start(nullptr), finish(nullptr), end_heap(nullptr) {}
		vector(size_type n, const_reference value) { initialize_fill(n, value); }
		explicit vector(size_type n) { initialize_fill(n, value_type()); }

	public:

		void push_back(const_reference value) {
			if (finish != end_heap) {
				construct(finish, value);
				++finish;
			}
			else insert_aux(finish, value);
		}

		void pop_back() {
			--finish;
			destroy(finish);
		}
	};


}


#endif // !__STL_VECTOR_H

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

		//插入元素，当容器已满时自动扩容，除push_back()调用过外 其他用途未知
		void insert_aux(iterator position,const_reference value);

		void deallocate() {
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
		iterator begin() const { return start; }
		const_iterator cbegin() const { return start; }
		iterator end() const { return finish; }
		const_iterator cend() const { return finish; }

		size_type size() const { return size_type(end() - begin()); }
		size_type capacity() const { return size_type(end_heap - start); }
		bool empty() const { return begin() == end(); }
		reference operator[](size_type n) { return *(begin() + n); }

		const_reference front() const { return *(begin()); }
		const_reference back() const { return *(end() - 1); }

	public:
		vector() :start(nullptr), finish(nullptr), end_heap(nullptr) {}
		vector(size_type n, const_reference value) { initialize_fill(n, value); }
		explicit vector(size_type n) { initialize_fill(n, value_type()); }

		~vector() {
			destroy(start,finish);
			deallocate();
		}
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

		void insert(iterator position, size_type n, const_reference value);

		iterator erase(iterator first, iterator last) {
			iterator res = copy(last, finish, first);
			destroy(res, finish);
			finish -= (last - first);
			return first;
		}

		iterator erase(iterator position) {
			if (position + 1 != finish) copy(position + 1, finish, position);
			--finish;
			destroy(finish);
			return position;
		}

		void resize(size_type new_size, const_reference value) {
			if (new_size < size())
				erase(begin() + new_size, end());
			else
				insert(end(), new_size - size(), value);
		}

		void resize(size_type new_size) { resize(new_size, value_type()); }

		void clear() { erase(begin(), end()); }
	};

	template<class T,class Alloc>
	void vector<T, Alloc>::insert_aux(iterator position, const_reference value) {
		if (finish != end_heap) {
			construct(finish, *(finish - 1));
			++finish;
			value_type value_copy = value;
			copy_back(position, finish - 2, finish - 1);
			*position = value_copy;
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::insert(iterator position, size_type n, const_reference value) {

	}

}


#endif // !__STL_VECTOR_H

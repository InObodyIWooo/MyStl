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
		typedef const pointer		const_iterator;
		typedef value_type*			iterator;
		typedef value_type&			reference;
		typedef const reference		const_reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
	protected:
		typedef m_alloc<value_type, Alloc> data_alloc;

		iterator start;		//已使用空间的头指针
		iterator finish;	//已使用空间的尾指针
		iterator end_heap;		//未使用空间的尾指针

	public:
		iterator begin()  { return start; }
		const_iterator cbegin() const { return start; }
		iterator end()  { return finish; }
		const_iterator cend() const { return finish; }

		size_type size()  { return size_type(end() - begin()); }
		size_type capacity() const { return size_type(end_heap - start); }
		bool empty() const { return begin() == end(); }
		reference operator[](size_type n) { return *(begin() + n); }

		const_reference front()  { return *(begin()); }
		const_reference back()  { return *(end() - 1); }

	protected:

		//插入元素，当容器已满时自动扩容，除push_back()调用过外 其他用途未知
		void insert_aux(iterator position,const_reference value);

		void deallocate() {
			if (start != nullptr)data_alloc::deallocate(start, capacity());
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
			iterator res = std::copy(last, finish, first);
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
			std::copy_backward(position, finish - 2, finish - 1);
			*position = value_copy;
		}
		else
		{
			size_type old_size = size(), new_size = size() == 0 ? 1 : 2 * size();

			iterator new_start = data_alloc::allocate(new_size);
			iterator new_finish = new_start;

			try
			{
				new_finish = uninitialized_copy(start, position, new_finish);
				construct(new_finish, value);
				++new_finish;
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (const std::exception&)
			{
				destroy(new_start, new_finish);
				data_alloc::deallocate(new_start, new_size);
				throw;
			}

			destroy(start, finish);
			deallocate();

			start = new_start;
			finish = new_finish;
			end_heap = new_start + new_size;
		}
	}

	template<class T, class Alloc>
	void vector<T, Alloc>::insert(iterator position, size_type n, const_reference value) {
		if (n == 0)return;

		if (capacity() - size() >= n)
		{
			value_type value_copy = value;
			size_type last_elem = finish - position;
			iterator old_finish = finish;
			if (last_elem > n) {
				uninitialized_copy(finish - n, finish, finish);
				finish += n;
				std::copy_backward(position, old_finish - n, old_finish);
				std::fill(position, position + n,value_copy);
			}
			else {
				uninitialized_fill_n(finish, n - last_elem, value_copy);
				finish += n - last_elem;
				uninitialized_copy(position, old_finish, finish);
				finish += last_elem;
				std::fill(position, old_finish, value_copy);
			}
		}
		else {
			size_type old_size = size();
			size_type new_size = old_size + std::max(old_size, n);//TODO: max()

			iterator new_start = data_alloc::allocate(new_size);
			iterator new_finish = new_start;

			try
			{
				new_finish = uninitialized_copy(start, position, new_finish);
				new_finish = uninitialized_fill_n(new_finish, n, value);
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (const std::exception&)
			{
				destroy(new_start, new_finish);
				data_alloc::deallocate(new_start, new_size);
				throw;
			}

			destroy(start, finish);
			deallocate();

			start = new_start;
			finish = new_finish;
			end_heap = new_start + new_size;
		}
	}

}


#endif // !__STL_VECTOR_H

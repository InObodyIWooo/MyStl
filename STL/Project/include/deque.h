#pragma once

#ifndef __STL_DEQUE_H
#define __STL_DEQUE_H

#include "stl_alloc.h"


namespace mystl
{
	inline size_t __deque_buff_size(size_t n, size_t value_size) { return n != 0 ? n / value_size : (512 < value_size ? 1 : 512 / value_size); }

	template<class T,class Ptr,class Ref,size_t BuffSize>
	struct _deque_iterator {
		typedef _deque_iterator<T, T*, T&, BuffSize> iterator;
		typedef _deque_iterator<T, const T*, const T&, BuffSize> const_iterator;
		typedef _deque_iterator self;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;
		typedef const reference const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef random_access_iterator_tag iterator_category;
		//map: 存储节点(指向缓冲区)
		typedef T** map_pointer;
		typedef map_pointer  map;
		static size_type buffer_size() { return __deque_buff_size(BuffSize, sizeof(value_type)); }

		map node;
		T* first;
		T* last;
		T* cur;

		_deque_iterator(const self& x) :node(x.node), first(x.first), last(x.last), cur(x.cur) {}

		//cur需要根据情况自行设置
		void set_node(const map& x)
		{
			node = x;
			first = *x;
			last = first + difference_type(buffer_size());
		}

		reference operator*() const { return *cur; }
		pointer operator->()const { return &(operator*()); }

		difference_type operator-(const self& x) const {
			return (x.last - x.cur) + (cur - first) +
				difference_type(buffer_size()) * (node - x.node - 1);
		}

		self& operator++() {
			++cur;
			if (cur == last) {
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}
		self operator++(int) const {
			self res = *this;
			++*this;
			return res;
		}

		self& operator--() {
			if (cur == first) {
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}
		self operator--(int) const {
			self res = *this;
			--*this;
			return res;
		}

		self& operator+=(difference_type n) {
			difference_type sum = n + cur - first;
			if (sum >= 0 && sum < difference_type(buffer_size()))cur += n;
			else {
				difference_type node_offset = sum > 0 ? sum / difference_type(buffer_size()) :
					-(difference_type((-sum - 1) / buffer_size()) + 1);
				set_node(node + node_offset);
				cur = first + sum - node_offset * difference_type(buffer_size());
			}
			return *this;
		}
		self& operator-=(difference_type n) { return *this += -n; }

		self operator+(difference_type n) const {
			self res = *this;
			return res += n;
		}
		self operator-(difference_type n) const {
			self res = *this;
			return res -= n;
		}

		reference operator[](difference_type n) const {
			return *(*this + n);
		}

		bool operator==(const self& x) const { return cur == x.cur; }
		bool operator!=(const self& x) const { return !(*this == x); }
		bool operator <(const self& x) const { return node == x.node ? cur < x.cur : node < x.node; }
	};

	template<class T,class Alloc = _alloc,size_t BuffSize = 0>
	class deque
	{
	public :
		typedef T value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef _deque_iterator<T, T*, T&, BuffSize> iterator;
		typedef _deque_iterator<T, const T*, const T&, BuffSize> const_iterator;
	protected:
		typedef m_alloc<value_type, Alloc> data_allocator;
		typedef m_alloc<pointer, Alloc> map_allocator;
		typedef pointer* map_pointer;
		map_pointer map;//存储node，node指向缓冲区(存储T元素)
		size_type map_size;
		iterator start;
		iterator finish;
	protected:
		size_type initial_map_size() { return size_type(8); }
		size_type buffer_size() { return iterator::buffer_size(); }
		pointer node_allocate() {
			pointer res = data_allocator::allocate(buffer_size());
			return res;
		}
		void create_map_node(size_type elements);
		void fill_initialize(size_type n, const_reference x);
		void reserve_map_at_back();
		void reserve_map_at_front();

		void push_back_aux(const_reference x);
		void push_front_aux(const_reference x);
		void pop_back_aux();
		void pop_front_aux();

	public:
		deque() :map(0), map_size(0), start(), finish() {  }
		deque(size_type n, const_reference x) :deque() { fill_initialize(n, x); }

	public:
		iterator begin() { return start; }
		iterator end() { return finish; }
		reference front() { return *begin(); }
		reference back() { return *(end() - 1); }
		bool empty() const { return start == finish; }
		size_type size() const { return size_type(finish - start); }

		void push_back(const_reference x) {
			if (finish.cur != finish.last - 1) {
				construct(finish.cur, x);
				++finish.cur;
			}
			else push_back_aux(x);
		}
		void push_front(const_reference x) {
			if (start.cur != start.first) {
				construct(start.cur - 1, x);
				++start.cur;
			}
			else push_front_aux(x);
		}
		void pop_back() {
			if (finish.cur != finish.first) {
				--finish.cur;
				destroy(finish.cur);
			}
			else pop_back_aux();
		}
		void pop_front() {
			if (start.cur != start.last - 1) {
				destroy(start.cur);
				++start.cur;
			}
			else pop_front_aux();
		}
		void insert(iterator position, const_reference x);
		void insert(iterator position, size_type n, const_reference x);
		template<class InputIterator,typename = std::enable_if<std::_Is_iterator<InputIterator>::value,int>::type>
		void insert(iterator position, InputIterator first, InputIterator last);

		iterator erase(iterator position);
		iterator erase(iterator first, iterator last);

	};

	template<class T,class Alloc,size_t BuffSize>
	void deque<T,Alloc,BuffSize>::fill_initialize(size_type n, const_reference x) {
		create_map_node(n);
		map_pointer cur;

		try
		{
			for (cur = start.node; cur < finish.node; ++cur) {
				uninitialized_fill(*cur, *cur + buffer_size(), x);
			}
			uninitialized_fill(finish.first, finish.cur, x);
		}
		catch (...)
		{
			throw;
		}
	}

	template<class T, class Alloc, size_t BuffSize>
	void deque<T, Alloc, BuffSize>::create_map_node(size_type elements) {
		size_type num_node = elements / buffer_size() + 1;
		map_size = max(num_node + 2, initial_map_size());
		map = map_allocator::allocate(map_size);

		map_pointer nstart = map + (map_size - num_node) / 2, nfinish = nstart + num_node - 1;
		map_pointer cur;
		try
		{
			for (cur = start.node; cur <= finish.node; ++cur) {
				*cur = node_allocate();
			}
		}
		catch (...)
		{
			for (cur = start.node; cur <= finish.node; ++cur) {
				data_allocator::deallocate(*cur);
			}
			throw;
		}

		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.last;
		finish.cur = finish.first + elements % buffer_size();
	}

	template<class T,class Alloc,size_t BuffSize>
	void deque<T, Alloc, BuffSize>::push_back_aux(const_reference x) {
		value_type x_copy = x;
		reserve_map_at_back();
		*(finish.node + 1) = node_allocate();
		try 
		{
			construct(finish.cur, x_copy);
			finish.set_node(finish.node + 1);
			finish.cur = finish.first;
		}
		catch (...)
		{
			data_allocator::deallocate(*finish.node);
			finish.set_node(finish.node - 1);
			finish.cur = finish.last - 1;
			destroy(finish.cur);
			throw;
		}
	}
	template<class T, class Alloc, size_t BuffSize>
	void deque<T, Alloc, BuffSize>::push_front_aux(const_reference x) {
		value_type x_copy = x;
		reserve_map_at_front();
		*(start.node - 1) = node_allocate();
		try
		{
			start.set_node(start.node - 1);
			start.cur = start.last - 1;
			construct(start.cur, x_copy);
		}
		catch (...)
		{
			destroy(start.cur);
			data_allocator::deallocate(*start.node);
			start.set_node(start.node + 1);
			start.cur = start.first;
			throw;
		}
	}
	template<class T, class Alloc, size_t BuffSize>
	void deque<T, Alloc, BuffSize>::pop_back_aux() {
		data_allocator::deallocate(*finish.node);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}
	template<class T, class Alloc, size_t BuffSize>
	void deque<T, Alloc, BuffSize>::pop_front_aux() {
		destroy(start.cur);
		data_allocator::deallocate(*start.node);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}
}

#endif 
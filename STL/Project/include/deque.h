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
		self operator++(int) {
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
		self operator--(int) {
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

		reference operator[](difference_type n) {
			return *(*this + n)
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
		void fill_initialize(size_type n, const_reference x);
		void create_map_node(size_type elements);
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

	};

}

#endif 
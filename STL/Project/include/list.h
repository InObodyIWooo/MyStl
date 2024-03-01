#pragma once

#ifndef __STL_LIST_H
#define __STL_LIST_H

#include "stl_alloc.h"

namespace mystl
{
	template<class T>
	struct _list_node
	{
		typedef _list_node<T>* pointer_node;
		pointer_node next;
		pointer_node prev;
		T data;
		_list_node(const T& x = T()) :next(nullptr), prev(nullptr), data(x) {}
	};

	template<class T, class Ptr,class Ref>
	struct _list_iterator : public iterator<T,bidirection_iterator_tag,Ptr,Ref>
	{
		typedef _list_iterator<T, T*, T&> iterator;
		typedef _list_iterator<T, Ptr, Ref> self;
		typedef _list_node<T>* pointer_node;
		typedef size_t size_type;
		using reference = mystl::iterator<T, bidirection_iterator_tag, Ptr, Ref>::reference;
		using pointer = mystl::iterator<T, bidirection_iterator_tag, Ptr, Ref>::pointer;
		using difference_type = mystl::iterator<T, bidirection_iterator_tag, Ptr, Ref>::difference_type;

		pointer_node node;

		_list_iterator() {}
		_list_iterator(const iterator& x) :node(x.node) {}
		_list_iterator(pointer_node x) :node(x) {}
		_list_iterator(const self& x) :node(x.node) {}

		bool operator ==(const self& x) { return node == x.node; }
		bool operator !=(const self& x) { return node != x.node; }

		reference operator*() const { return (*node).data; }
		pointer operator->() const { return  &(operator*()); }

		self& operator++() {
			node = (*node).next;
			return *this;
		}

		self& operator++(int) {
			self tmp = *this;
			++(*this);
			return tmp;
		}

		self& operator--() {
			node = (*node).prev;
			return *this;
		}

		self& operator--(int) {
			self tmp = *this;
			--(*this);
			return tmp;
		}

	};

	template<class T,class Alloc = _alloc>
	class list
	{
	public:
		typedef T					value_type;
		typedef T& reference;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		typedef _list_iterator<T, T*, T&> iterator;
		typedef _list_iterator<T, const T*, const T&> const_iterator;
		typedef _list_node<T> list_node;
		typedef list_node* pointer_node;

		typedef m_alloc<list_node, Alloc> data_allocator;
	protected:
		pointer_node last_node;//end of list
		size_type size_list;
	protected:
		pointer_node get_node() { return data_allocator::allocate(); }
		void put_node(pointer_node p) { data_allocator::deallocate(p); }

		pointer_node create_node(const value_type & x)
		{
			pointer_node res = get_node();
			construct(&res->data, x);
			return res;
		}

		void destroy_node(pointer_node p)
		{
			destroy(&p->data);
			put_node(p);
		}

		void empty_initialize() { last_node = get_node(); size_list = 0; last_node->next = last_node; last_node->prev = last_node; }

		void link_node_at_back(pointer_node p)
		{
			p->prev = last_node->prev;
			(last_node->prev)->next = p;
			last_node->prev = p;
			p->next = last_node;
			++size_list;
		}

		pointer_node delet_node_from_back()
		{
			pointer_node res = last_node->prev;
			last_node->prev = res->prev;
			res->prev->next = last_node;
			--size_list;
			return res;
		}
		void fill_list(size_type n, const value_type& x)
		{
			for (;n > 0 ; --n)
			{
				pointer_node t = create_node(x);
				link_node_at_back(t);
			}
		}

		template<class InputIterator>
		void copy_list(InputIterator first, InputIterator last)
		{
			size_type n = distance(first, last);
			for (; n > 0; --n,++first)
			{
				pointer_node t = create_node(*first);
				link_node_at_back(t);
			}
		}

	public:
		iterator begin() { return last_node->next; }
		const_iterator cbegin() const { return last_node->next; }
		iterator end() { return last_node; }
		const_iterator cend() const { return last_node; }

		bool empty() { return last_node->next == last_node; }
		size_type size() { return size_list; }

		reference front() { return *begin(); }
		reference back() { return *end(); }
	public:
		list() { empty_initialize(); }
		list(const list& left) : list() { copy_list(left.begin(), lefe.end()); }
		//list(list&& right) {}
		list(size_type n) : list() { fill_list(n, value_type()); }
		list(size_type n, const T& x) : list() { fill_list(n, x); }
		template<class InputIterator>
		list(InputIterator first, InputIterator last) : list() { copy_list(first, last); }



	};
}

#endif // !__STL_LIST_H

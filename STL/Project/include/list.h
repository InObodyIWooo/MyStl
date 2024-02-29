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
		typedef _list_node<T> self;
		pointer_node next;
		pointer_node prev;
		T data;
		_list_node(const T& x = T()) :next(nullptr), prev(nullptr), data(x) {}
	};

	template<class T, class Ptr,class Ref>
	struct _list_iterator : iterator<T, bidirection_iterator_tag,Ptr,Ref>
	{
		typedef _list_iterator<T, T*, T&> iterator;
		typedef _list_iterator<T, Ptr, Ref> self;
		typedef _list_node<T>* pointer_node;
		typedef size_t size_type;

		pointer_node node;

		_list_iterator() {}
		_list_iterator(const iterator& x) :node(x.node) {}
		_list_iterator(pointer_node x) :node(x) {}
		_list_iterator(const self& x) :node(x.node) {}

		bool operator ==(const self& x) { return node == x.node; }
		bool operator !=(const self& x) { return node != x.node; }

		reference operator*()const { return (*node).data }
		pointer operator->()const { return &(operator*()); }

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
}
#endif // !__STL_LIST_H

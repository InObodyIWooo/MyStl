#pragma once

#ifndef __STL_LIST_H
#define __STL_LIST_H

#include "../stl_alloc.h"

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
		using iterator_category = mystl::iterator<T, bidirection_iterator_tag, Ptr, Ref>::iterator_category;

		pointer_node node;

		_list_iterator() {}
		_list_iterator(pointer_node x) :node(x) {}
		template<class InputIterator>
		_list_iterator(const InputIterator& x) : node(&(*x)) {}

		self& operator= (const pointer_node& x) { node = x; return *this; }
		bool operator ==(const self& x) const { return node == x.node; }
		bool operator !=(const self& x) const { return node != x.node; }

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
		typedef T&					reference;
		typedef const T&			const_reference;
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

		pointer_node create_node(const_reference x)
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

		iterator link_node_at_pos(iterator position, pointer_node p)
		{
			pointer_node pos = position.node;
			p->prev = pos->prev;
			(pos->prev)->next = p;
			pos->prev = p;
			p->next = pos;
			++size_list;
			return p;
		}

		iterator delet_node_from_pos(iterator position)
		{
			pointer_node res = position.node->next;
			res->prev = position.node->prev;
			position.node->prev->next = res;
			destroy_node(position.node);
			--size_list;
			return res;
		}
		iterator fill_list(iterator position ,size_type n, const_reference x)
		{
			iterator res = position;
			while(n--)
			{
				pointer_node t = create_node(x);
				res = link_node_at_pos(res,t);
			}
			return res;
		}

		template<class InputIterator>
		iterator copy_list(iterator position, InputIterator first, InputIterator last)
		{
			iterator res = position;
			size_type n = distance(first, last);
			while(n--)
			{
				pointer_node t = create_node(*(--last));
				res = link_node_at_pos(res,t);
			}
			return res;
		}

		void transfer(iterator position, iterator first, iterator last) {
			if (position != last && first != last)
			{
				size_list += distance(first, last);
				iterator tmp = last.node->prev;
				first.node->prev->next = last.node;
				last.node->prev = first.node->prev;
				position.node->prev->next = first.node;
				first.node->prev = position.node->prev;
				position.node->prev = tmp.node;
				tmp.node->next = position.node;
			}

		}
		pointer_node _sort(pointer_node& first, size_type n);
	public:
		iterator begin() { return last_node->next; }
		const_iterator cbegin() const { return last_node->next; }
		iterator end() { return last_node; }
		const_iterator cend() const { return last_node; }

		bool empty() { return last_node->next == last_node; }
		size_type size() { return size_list; }

		reference front() { return *begin(); }
		reference back() { return *(--end()); }
	public:
		list() { empty_initialize(); }
		list(const list& left) : list() { copy_list(begin(), left.begin(), left.end()); }
		//list(list&& right) {}
		explicit list(size_type n) : list() { fill_list(begin(), n, value_type()); }
		list(size_type n, const_reference x) : list() { fill_list(begin(), n, x); }
		template<class InputIterator>
		list(InputIterator first, InputIterator last) : list() { copy_list(begin(), first, last); }

		iterator insert(iterator position, const_reference x);
		iterator insert(iterator position, size_type n, const_reference x);

		//只有当InputItrator为迭代器时，enable_if才会有type，调用模板实例化
		template<class InputIterator, typename = std::enable_if<std::_Is_iterator<InputIterator>::value, int>::type>
		iterator insert(iterator position, InputIterator first, InputIterator last);

		iterator erase(iterator position);
		iterator erase(iterator firsr, iterator last);

		void swap(list& x)
		{
			iterator first = begin();
			size_type t = size();
			transfer(begin(), x.begin(), x.end());
			x.size_list = 0;
			x.transfer(x.begin(), first, end());
			size_list -= t;
		}

		void clear();
		//除去与x相同的元素
		void remove(const_reference x);
		//除去连续相同的元素
		void unique();

		//x不能与*this相同
		void splice(iterator position, list& x);
		void splice(iterator position, /*list&,*/ iterator i);
		//position不能位于[first,last)之内
		void splice(iterator position, /*list&,*/ iterator first, iterator last);

		//将x合并到*this，两个list必须是有序的且为递增
		void merge(list& x);
		void reverse();
		void sort() { _sort(last_node->next, size_list); }

	public:

		void push_back(const_reference x) { insert(end(), x); }
		void push_front(const_reference x) { insert(begin(), x); }
		void pop_back() { erase(--end()); }
		void pop_front() { erase(begin()); }
	};


	template<class T,class Alloc>
	typename list<T,Alloc>::iterator 
		list<T,Alloc>::insert(iterator position, const_reference x) {
		pointer_node p = create_node(x);
		return link_node_at_pos(position, p);
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(iterator position, size_type n, const_reference x) {
		return fill_list(position, n, x);
	}

	template<class T, class Alloc> template<class InputIterator,typename>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
		return copy_list(position, first, last);
	}

	template<class T,class Alloc>
	typename list<T,Alloc>::iterator
		list<T, Alloc>::erase(iterator position) {
		return delet_node_from_pos(position);
	}

	template<class T, class Alloc>
	typename list<T, Alloc>::iterator
		list<T, Alloc>::erase(iterator first, iterator last) {
		size_type n = distance(first, last);
		iterator tmp = first;
		while(n--)tmp = delet_node_from_pos(tmp);
		return last;
	}

	template<class T,class Alloc>
	void list<T, Alloc>::clear() {
		erase(begin(), end());
	}

	template<class T,class Alloc>
	void list<T, Alloc>::remove(const_reference x) {
		iterator cur = begin();
		while (cur != end())
		{
			iterator tmp = cur++;
			if (*tmp == x)delet_node_from_pos(tmp);
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::unique() {
		iterator cur = begin();
		iterator next = cur;
		while (++next != end())
		{
			if (*cur == *next)erase(next);
			else cur = next;
			next = cur;
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list& x) {
		if (!x.empty())
			transfer(position, x.begin(), x.end());
	}

	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, /*list&,*/ iterator i) {
		iterator last = i;
		++last;
		if (position == i || position == last)return;
		transfer(position, i, last);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, /*list&,*/ iterator first, iterator last) {
		if (first != last)
			transfer(position, first, last);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::merge(list& x) {
		iterator first1 = begin(), last1 = end(),first2 = x.begin(), last2 = x.end();

		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *first1) {
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else ++first1;
		}
		if (first2 != last2)transfer(last1, first2, last2);
		x.size_list = 0;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::reverse() {
		iterator cur = begin();
		++cur;
		while (cur != end())
		{
			iterator old = cur;
			++cur;
			transfer(begin(), old, cur);
		}
	}

	template<class T, class Alloc>
	typename list<T,Alloc>::pointer_node
		list<T, Alloc>::_sort(pointer_node& first, size_type n) {
		//sort [first,first + n) return first + n
		if (!n)return first;
		else if (n == 1)return first->next;
		pointer_node mid = _sort(first, n / 2);
		pointer_node last = _sort(mid, n - n / 2);
		iterator i = first, j = mid,newhead = first->prev;
		while (i != mid && j != last)
		{
			if (*j < *i)
			{
				iterator old = j;
				++j;
				transfer(i, old, j);
			}
			else ++i;
		}
		first = newhead.node->next;
		return last;
	}

}

#endif // !__STL_LIST_H

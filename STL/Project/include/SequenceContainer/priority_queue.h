#pragma once

#ifndef __STL_PRIORITY_QUEUE_H
#define __STL_PRIORITY_QUEUE_H

#include "heap.h"
#include "vector.h"

namespace mystl
{
	template<class T,class Sequence = vector<int>,class Compare = std::less<typename Sequence::value_type>>
	class priority_queue
	{
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::difference_type difference_type;
	protected:
		Sequence container;
		Compare cmp;
	public:
		priority_queue() :container() {};
		explicit priority_queue(const Compare& x) :container(), cmp(x) {};

		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last) :container(first, last) {
			mystl::make_heap(container.begin(), container.end(), cmp);
		}
		template<class InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare& x) : container(first, last), cmp(x) {
			mystl::make_heap(container.begin(), container.end(), cmp);
		}

	public:
		bool empty()const { return container.empty(); }
		size_type size() const { return container.size(); }
		const_reference top() const { return container.front(); }
		void push(const value_type& x) {
			try
			{
				container.push_back(x);
				mystl::push_heap(container.begin(), container.end(), cmp);
			}
			catch (...)
			{
				container.clear();
				throw;
			}

		}
		void pop() {
			try
			{
				mystl::pop_heap(container.begin(), container.end(), cmp);
				container.pop_back();
			}
			catch (...)
			{
				container.clear();
				throw;
			}
		}
	};

}
#endif
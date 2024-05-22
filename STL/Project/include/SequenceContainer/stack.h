#pragma once

#ifndef __STL_STACK_H
#define __STL_STACK_H

#include "deque.h"

namespace mystl
{
	template<class T,class Sequence = deque<T>>
	class stack
	{
	public:
		typedef typename Sequence::value_type value_type;
		typedef typename Sequence::size_type size_type;
		typedef typename Sequence::reference reference;
		typedef typename Sequence::const_reference const_reference;
	protected:
		Sequence container;
	public:
		bool empty() const { return container.empty(); }
		size_type size() const { return container.size(); }
		reference top()  { return container.back(); }
		const_reference top() const { return container.back(); }
		void push(const value_type& x) { container.push_back(x); }
		void pop() { container.pop_back(); }
	};

}

#endif
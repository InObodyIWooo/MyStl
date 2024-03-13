

#include <iostream>

#include "../include/stl_alloc.h"
#include "../include/stl_construct.h"
#include "../include/stl_uninitialized.h"
#include "../include/vector.h"
#include "../include/list.h"
#include "../include/deque.h"
#include "../include/stack.h"
#include "../include/queue.h"
#include "../include/heap.h"


#include <vector>
#include <list>
#include <deque>
#include <stack>
#include <algorithm>
#include <Windows.h>
#include <time.h>
using namespace std;


int main()
{
	vector<int> a;

	for (int i = 0; i < 10; i++)
	{
		a.push_back(10 - i);
		cout << a.back() << ' ';
		mystl::push_heap(a.begin(), a.end(),greater<int>());
	}
	puts("");
	for (auto c : a)cout << c << ' ';
	puts("");
	for (int i = 0; i < 10; i++)
	{
		cout << a.front() << ' ';
		mystl::pop_heap(a.begin(), a.end(), greater<int>());
		a.pop_back();
	}

	return 0;
}
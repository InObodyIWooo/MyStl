

#include <iostream>

#include "../include/stl_alloc.h"
#include "../include/stl_construct.h"
#include "../include/stl_uninitialized.h"
#include "../include/vector.h"
#include "../include/list.h"
#include "../include/deque.h"

#include <vector>
#include <list>
#include <deque>
#include <algorithm>
#include <Windows.h>
#include <time.h>
using namespace std;


int main()
{
	mystl::deque<int> a;
	mystl::deque<char> b;
	//deque<int>a;
	//deque<char>b;
	for (int i = 0; i < 10; i++)a.push_back(i+1),b.push_back(char('a' + i));

	a.insert(a.begin(), b.begin() + 5, b.end());

	for (auto i = a.begin(); i != a.end(); ++i)cout << *i << ' ';
	//for (auto c : a)cout << c << ' ';
	puts("");


	//for (auto c : a)cout << c << ' ';
	//puts("");

	//vector<int> a;
	//mystl::vector<int> b;

	//for (int i = 0; i < 10; i++)b.push_back(i + 1);

	//for (auto c : b)cout << c << ' ';
	//puts("");
	//b.pop_back();
	//cout << b.size() << ' ' << b.capacity() << endl;
	//cout << b.back() << endl;

	//b.clear();
	//cout << b.size() << ' ' << b.capacity() << endl;
	//b.resize(20, 100);

	//cout << b.size() << ' ' << b.capacity() << endl;

	//b.resize(8);
	//cout << b.size() << ' ' << b.capacity() << endl;
	//for (auto c : b)cout << c << ' ';
	//puts("");
	//cout << b.front() << endl;
	return 0;
}
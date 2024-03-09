

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
	mystl::list<int> a,b;
	//list<int> a;
	for (int i = 0; i < 1e6; i++)a.push_front(i + 1);
	//for (auto c : a)cout << c << ' ';
	//puts("");

	int c = 10, d;
	cout << ((d = c)++) << endl;

	cout << d << endl;
	clock_t time_start, time_finish;
	time_start = clock();
	//a.sort();
	time_finish = clock();
	cout << "运行时间: " << double(time_finish - time_start) / CLOCKS_PER_SEC << endl;


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
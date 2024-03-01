

#include <iostream>

#include "../include/stl_alloc.h"
#include "../include/stl_construct.h"
#include "../include/stl_uninitialized.h"
#include "../include/vector.h"
#include "../include/list.h"

#include <vector>
#include <list>
using namespace std;


int main()
{
	vector<int> a;
	mystl::vector<int> b;

	for (int i = 0; i < 10; i++)b.push_back(i + 1);

	for (auto c : b)cout << c << ' ';
	puts("");
	b.pop_back();
	cout << b.size() << ' ' << b.capacity() << endl;
	cout << b.back() << endl;

	b.clear();
	cout << b.size() << ' ' << b.capacity() << endl;
	b.resize(20, 100);

	cout << b.size() << ' ' << b.capacity() << endl;

	b.resize(8);
	cout << b.size() << ' ' << b.capacity() << endl;
	for (auto c : b)cout << c << ' ';
	puts("");
	cout << b.front() << endl;
	return 0;
}
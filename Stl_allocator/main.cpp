

#include <iostream>
#include <vector>

#include "stl_uninitialized.h"
#include "stl_alloc.h"

using namespace std;


int main()
{
	int* p = mystl::MyAlloc<int, mystl::_alloc>::allocate();

	*p = 2;

	cout << *p << endl;

	mystl::MyAlloc<int, mystl::_alloc>::deallocate(p);


	int *p2 = mystl::MyAlloc<int, mystl::_alloc>::allocate(16);

	for (int i = 0; i < 16; i++)p2[i] = i + 1;

	for (int i = 0; i < 16; i++)cout << p2[i] << ' ';
	puts("");

	//mystl::MyAlloc<int, mystl::_alloc>::deallocate(p2,16);



	return 0;
}
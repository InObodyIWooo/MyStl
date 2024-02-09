

#include <iostream>
#include <vector>

#include "stl_uninitialized.h"


using namespace std;

bool fuc(std::true_type)
{
	return true;
}

bool fuc(std::false_type)
{
	return false;
}

int main()
{
	return 0;
}
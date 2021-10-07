#include <iostream>
using namespace std;
#include "RangeArray.h"

RangeArray::RangeArray(int s, int e) :Array(e - s + 1)
{
	low = s;
	high = e;
}

RangeArray::~RangeArray()
{

}

int RangeArray::baseValue()
{
	return low;
}
int RangeArray::endValue()
{
	return high;
}
int& RangeArray::operator [](int i)
{
	static int tmp;
	if (i >= low && i <= high) {
		return Array::data[i - low];
	}
	else {
		cout << " Array bound error!" << endl;
		return tmp;
	}
}
int RangeArray::operator [](int i) const
{
	return Array::operator [](i - low);
}

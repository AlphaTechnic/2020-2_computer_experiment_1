#include "Array.h"
#include <iostream>
using namespace std;

template <class T>
class GrowableArray : public Array<T>
{
public:
	GrowableArray(int size) : Array<T>(size) {}
	~GrowableArray() {}

	T& operator[] (int i){
		static T tmp;
		if (i >= 0 && i < this->len) {
			return this->data[i];
		}
		else if (i >= this->len){
			T* data_tmp;
			int j;
			data_tmp = new T[i * 2];  // 2배의 저장공간 할당
			for (j = 0; j < this->len; j++)
				data_tmp[j] = this->data[j];	
			for (j = this->len; j < i * 2; j++)
				data_tmp[j] = 0;	// 추가된 공간은 0으로 초기화
			this->len = i * 2;	// 크기 상태 변수 갱신
			delete[] this->data;	// 기존 저장 공간 메모리 할당 해제
			this->data = data_tmp;
			return this->data[i];
		}
		else{
			cout << "Array bound error!" << endl;
			return tmp;
		}
	}
};
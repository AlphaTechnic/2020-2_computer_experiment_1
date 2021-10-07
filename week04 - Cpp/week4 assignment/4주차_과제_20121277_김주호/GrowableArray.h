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
			data_tmp = new T[i * 2];  // 2���� ������� �Ҵ�
			for (j = 0; j < this->len; j++)
				data_tmp[j] = this->data[j];	
			for (j = this->len; j < i * 2; j++)
				data_tmp[j] = 0;	// �߰��� ������ 0���� �ʱ�ȭ
			this->len = i * 2;	// ũ�� ���� ���� ����
			delete[] this->data;	// ���� ���� ���� �޸� �Ҵ� ����
			this->data = data_tmp;
			return this->data[i];
		}
		else{
			cout << "Array bound error!" << endl;
			return tmp;
		}
	}
};
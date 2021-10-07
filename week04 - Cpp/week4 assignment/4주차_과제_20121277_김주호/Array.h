#ifndef __ARRAY__
#define __ARRAY__

#include <iostream>
using namespace std;

template <class T>

class Array {
	protected:
		T* data;
		int len;
	public:
		Array(int size);
		~Array();
		int length();
		virtual T& operator[] (int i);
		T operator[](int i) const;
		void print();
	};

	template <class T>
	Array<T>::Array(int size) {
		data = NULL;
		if (size < 0) {
			cout << "Invalid size!" << endl;
		}
		else {
			len = size;
			data = new T[len];
		}
	}

	template <class T>
	Array<T>::~Array() {
		if (data != NULL) {
			delete[] data;
		}
	}

	template <class T>
	int Array<T>::length() {
		return len;
	}

	template <class T>
	T& Array<T>::operator[](int i) {
		static T tmp;
		if (i > len - 1 || i < 0) {
			cout << "Array bound error!" << endl;
			return tmp;
		}
		else {
			return data[i];
		}
	}

	template <class T>
	T Array<T>::operator[](int i) const {
		static T tmp;
		if (i > len - 1 || i < 0) {
			cout << "Array bound error!" << endl;
			return tmp;
		}
		else {
			return data[i];
		}
}

template <class T>
void Array<T>::print() {
	int i;
	cout << "[";
	for (i = 0; i < len; i++) {
		cout << " " << data[i];
	}
	cout << "]" << endl;
}
#endif
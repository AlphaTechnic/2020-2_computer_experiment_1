#include "Str.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

Str::Str(int leng) {
	len = leng;
}

Str::Str(const char* neyong) {
	str = new char[strlen(neyong) + 1]; // \n이 들어갈 메모리도 확보
	strcpy(str, neyong);
	len = strlen(str);
}

Str::~Str() {
	delete []str;
}

int Str::length(void) {
	return len;
}

char* Str::contents(void) {
	return str;
}

int Str::compare(class Str& a) {
	return strcmp(str, a.contents());
}

int Str::compare(const char* a) {
	return strcmp(str, a);
}

void Str::operator=(const char* a) {
	if (strlen(a) < strlen(str)) {
		delete[] str;
		str = new char[strlen(a) + 1];
	}
	strcpy(str, a);
	len = strlen(a);
}

void Str::operator=(class Str& a) {
	if (a.length() < strlen(str)) {
		delete[] str;
		str = new char[strlen(a.contents()+1)];
	}
	strcpy(str, a.contents());
	len = a.length();
}
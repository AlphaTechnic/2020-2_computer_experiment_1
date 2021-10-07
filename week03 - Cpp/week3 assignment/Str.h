#ifndef __STR__
#define __STR__

class Str {
	private:
		char* str;
		int len;
	public:
		Str(int leng); // string의 길이
		Str(const char* neyong); // 초기화할 내용이 들어감
		~Str(); // 소멸자
		int length(void);// string의 길이를 리턴
		char* contents(void); // string의 내용을 리턴
		int compare(class Str& a); // a와 strcmp
		int compare(const char* a); // a와 strcmp
		void operator=(const char* a); // 문자열 파라미터로 전달
		void operator=(class Str& a); // Str객체 파라미터로 전달
};
#endif
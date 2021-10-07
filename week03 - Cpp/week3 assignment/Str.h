#ifndef __STR__
#define __STR__

class Str {
	private:
		char* str;
		int len;
	public:
		Str(int leng); // string�� ����
		Str(const char* neyong); // �ʱ�ȭ�� ������ ��
		~Str(); // �Ҹ���
		int length(void);// string�� ���̸� ����
		char* contents(void); // string�� ������ ����
		int compare(class Str& a); // a�� strcmp
		int compare(const char* a); // a�� strcmp
		void operator=(const char* a); // ���ڿ� �Ķ���ͷ� ����
		void operator=(class Str& a); // Str��ü �Ķ���ͷ� ����
};
#endif
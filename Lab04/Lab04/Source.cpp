#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string calc_mas(int* mas, int siz, int b, int d) {
	int result = 0;
	int err = 0;
	_asm {
		xor esi, esi	; �������� ������� ������� � �������
		xor edi, edi	; �������� ������� �����

		mov ebx, mas	; ebx ��������� �� ������ �������
		mov ecx, siz	; ������� ����� := ���������� ��������� �������
		
		;;;;; j�cxz ex	; ���������, ���� ����� ������� 0
		
		or ecx, ecx		; ����� �������� ecx � 0
		jz ex			; ���� 0, �� ������ �� ����� ������
		jns begin_loop	; ���� �� 0 � �� �������������, �������� ����

		mov err, 1		; ��������� ������ � ����� �������
		jmp fin			; ����� �� �������
		
		begin_loop:
			mov eax, [ebx + ecx * 4 - 4]	; ���������� ������� �������
			mov edx, d					; ���������� ��������� � d
			cmp eax, edx				; ��������� a[i] � d
			jg end_loop					; ���� ������, �� ��������� ����
			mov edx, b					; ���������� ��������� � b
			cmp eax, edx				; ��������� a[i] � b
			jl end_loop					; ���� ������, �� ��������� ����
			
			add edi, eax				; ������� ������������� �������, ���������� ��� � �����
			jno end_loop				; ���� ��� ������������, ���� ������

			mov err, 2					; ��������� ������ ������������
			jmp fin						; ������� �� �������

		end_loop :
			inc esi						; ��������� � ���������� ��������
			loop begin_loop				; ��������� ���� ��� ���� ��������� �������
		ex : 
			mov eax, edi				; ���������� ����� ���������
			mov result, eax
		fin	:
	}

	string ans;

	if (err == 1) {
		ans = "Negative array size";
	}
	else if (err == 2) {
		ans = "Overflow";
	}
	else {
		ans = to_string(result);
	}
	return ans;
}



int main() {
	int n;
	cout << "Input array size: ";
	cin >> n;
	int* a = new int [n];
	cout << "Input array: ";
	for (int i = 0; i < n; i++)
	{
		cin >> a[i];
	}
	int b, d;
	cout << "Input b = ";
	cin >> b;
	cout << "Input d = ";
	cin >> d;

	cout << "Result: " << calc_mas(a, n, b, d) << endl;
	system("pause");
	return 0;
}

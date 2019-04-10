/*
__asm{
	mov	e�x	,	a; <e�x>=a
	mov	ebx	,	b; <ebx>=b
	cmp	e�x	,	ebx; ��������� a � b
	jg	l_bigger; ������� ���� a>b
	jl	l_smaller; ������� ���� a<b
	mov	eax	,	25;<eax>=25
	jmp	exit; ������� �� ����� ���������
l_bigger:
	or	ebx	,	ebx; ��������� b � 0
	jz	errorZ; ������ ������� �� ����
	mov	eax	,	e�x; <eax>=a
	cdq;	���������� ������� <edx:eax> = a
	; ��������� �������� �������� ������
	idiv 	ebx; <eax> = a/b
	sub	eax	,	4; <eax> = a/b � 4
	jo	errorOF
	jmp	exit; ������� �� ����� ���������
l_smaller:
	or	ecx	,	ecx; ��������� b � 0
	jz	error; ������ ������� �� ����
	mov	eax	,	ebx; <eax>=b
	imul 	ecx; <edx:eax> = b*b
	add	eax	,	-5; ���������� � ������� �����
	; <eax> = b*b � 5
	adc	edx	,	-1; ��������� ������� �����
	; <edx:eax> = b*b � 5
	; ��������� �������� �������� ������
	idiv	ecx; < eax> = (b*b � 5)/a
	jmp exit
errorZ:
errorOF:
exit:
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string calc_asm(int a, int b) {
	int result = 0;
	int err = 0;
	_asm {
		mov	ecx, a			; <ecx> = a
		mov	ebx, b			; <ebx> = b
		cmp	ecx, ebx		; ��������� a � b
		jg l_bigger			; �������, ���� a > b
		jl l_smaller		; �������, ���� a < b
		mov	eax, 6			; <eax> = 6
		jmp	ex				; ������� �� ����� ���������

		l_bigger :
			or ecx, ecx		; ��������� a � 0
			jz errorZ		; ������ ������� �� ����
			mov	eax, ecx	; <eax> = a
			imul ecx		; <edx:eax> = a * a
			jo errorOF		; ������ ������������
			sub eax, ebx	; <eax> = a * a - b
			jo errorOF		; ������ ������������
			cdq				; <edx:eax> = a * a - b
			idiv ecx		; <eax> = (a * a - b) / a

			add eax, -3		; <eax> = (a * a - b) / a - 3
			jo errorOF		; ������ ������������

			jmp	ex			; ������� �� ����� ���������
		
		l_smaller :
			or ebx, ebx		; ��������� b � 0
			jz errorZ		; ������ ������� �� ����
			
			mov	eax, ecx	; <eax> = a
			imul ecx		; <edx:eax> = a * a
			jo errorOF		; ������ ������������
			add eax, -3		; <eax> = a * a - 3
			jo errorOF		; ������ ������������
			cdq				; <edx:eax> = a * a - 3
			idiv ebx		; <eax> = (a * a - 3) / b

			jmp ex			; ������� �� ����� ���������

		; ������ ������� �� ���� - ���������
		errorZ :
			mov err, 1
			jmp fin

		; ������ ������������ - ���������
		errorOF:
			mov err, 2
			jmp fin

		ex:
			mov result, eax

		fin:
	}

	string ans;

	if (err == 1) {
		ans = "Division by 0";
	}
	else if (err == 2) {
		ans = "Overflow";
	}
	else {
		ans = to_string(result);
	}
	return ans;
}

int main(){
	int a, b;
	cout << "Input a, b: ";
	cin >> a >> b;
	cout << calc_asm(a, b) << endl;
	system("pause");
	return 0;
}
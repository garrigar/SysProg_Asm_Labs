/*
__asm{
	mov eax , a ; <eax>=a*a-l
	mov ebx , b
	mov ecx , c

	imul eax ; <edx:eax> = a*a
	dec eax ; <eax>=a*a-l
	add eax , ebx ; <eax>=a*a-1+b
	jz error_zf ; ошибка деление на ноль
	jo error_of ; ошибка переполнения

	push eax ; в стеке a*a-1+b
	mov eax, 25; <eax>=25

	; готовимся к делению
	cdq ; eax ==> edx:eax
	idiv ecx ; <eax>=25/c
	sub eax , ebx ; <eax>=25/c-b
	jo error_of ; ошибка переполнения
	add eax , 2 ; <eax>=25/c-d+2
	jo error_of ; ошибка переполнения

	; готовимся к делению
	pop ebx ; <ebx>= a*a-1+b
	cdq ; eax ==> <edx:eax>=25/c-d+2
	; добавить проверку ситуации «частное велико»
	idiv ebx ; <eax>=(25/c-d+2)/( a*a-1+b)
}
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// (-2*с + d*82)/(a/4 - 1);
string calc_asm(int a, int c, int d) {
	int result = 0;
	int err = 0;
	_asm {
		mov eax, a	; <eax> = a
		mov ebx, 4	; <ebx> = 4
		cdq			; <edx:eax> = a
		idiv ebx	; <eax> = a / 4

		dec eax		; <eax> = a / 4 - 1
		jnz cont1

		mov err, 1	;
		jmp ex

		cont1:
		push eax	; stack = { (a / 4 - 1) }
		
		mov eax, d	; <eax> = d
		mov ebx, 82	; <ebx> = 82
		imul ebx	; <eax> = d * 82
		jno cont2	;

		mov err, 2	;
		pop ecx		; уравниваем количество push и pop
		jmp ex		;

		cont2:
		push eax	; stack = { (d * 82), (a / 4 - 1) }

		mov eax, c	; <eax> = c
		mov ebx, -2	; <ebx> = -2
		imul ebx	; <eax> = c * (-2)
		jno cont3	;

		mov err, 2	;
		pop ecx		; уравниваем количество push и pop
		pop ecx		; уравниваем количество push и pop
		jmp ex		;

		cont3:
		pop ebx		; <ebx> = d * 82, stack = { (a / 4 - 1) }
		add eax, ebx; <eax> = c * (-2) + d * 82
		jno cont4

		mov err, 2	;
		pop ecx		; уравниваем количество push и pop
		jmp ex		;

		cont4:
		pop ebx		; <ebx> = a / 4 - 1, stack empty
		cdq			; <edx:eax> = c * (-2) + d * 82
		idiv ebx	; <eax> = (c * (-2) + d * 82)/(a / 4 - 1)

		mov result, eax;

		ex:
	}

	string ans;

	if (err == 1) {
		ans = "Denominator is 0";
	}
	else if (err == 2) {
		ans = "Overflow";
	}
	else {
		ans = to_string(result);
	}
	return ans;
}

// (-2*с + d*82)/(a/4 - 1);
string calc_cpp(int a, int c, int d) {
	if (a / 4 - 1 == 0)
		return "Denominator is 0";
	long long a_l = a;
	long long c_l = c;
	long long d_l = d;
	
	long long res = (-2 * c_l + d_l * 82) / (a_l / 4 - 1);

	return to_string(res);

}

int main() {
	ifstream fin("input.txt");
	ofstream fout("output.txt");

	int a, c, d;

	string temp;
	
	fin >> temp;
	try {
		a = stoi(temp);
	}
	catch (invalid_argument) {
		fout << "a: invalid argument" << endl;
		return -1;
	}
	catch (out_of_range) {
		fout << "a: out of range" << endl;
		return -1;
	}
	fin >> temp;
	try {
		c = stoi(temp);
	}
	catch (invalid_argument) {
		fout << "c: invalid argument" << endl;
		return -1;
	}
	catch (out_of_range) {
		fout << "c: out of range" << endl;
		return -1;
	}
	fin >> temp;
	try {
		d = stoi(temp);
	}
	catch (invalid_argument) {
		fout << "d: invalid argument" << endl;
		return -1;
	}
	catch (out_of_range) {
		fout << "d: out of range" << endl;
		return -1;
	}

	fout << "asm: " << calc_asm(a, c, d) << endl;
	fout << "c++: " << calc_cpp(a, c, d) << endl;

	fin.close();
	fout.close();

	//system("pause");
	return 0;

}
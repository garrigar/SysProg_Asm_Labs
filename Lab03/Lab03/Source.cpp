/*
__asm{
	mov	eсx	,	a; <eсx>=a
	mov	ebx	,	b; <ebx>=b
	cmp	eсx	,	ebx; сравнение a и b
	jg	l_bigger; переход если a>b
	jl	l_smaller; переход если a<b
	mov	eax	,	25;<eax>=25
	jmp	exit; переход на конец программы
l_bigger:
	or	ebx	,	ebx; сравнение b и 0
	jz	errorZ; ошибка деление на ноль
	mov	eax	,	eсx; <eax>=a
	cdq;	подготовка делени€ <edx:eax> = a
	; обработка ситуации Ђчастное великої
	idiv 	ebx; <eax> = a/b
	sub	eax	,	4; <eax> = a/b Ц 4
	jo	errorOF
	jmp	exit; переход на конец программы
l_smaller:
	or	ecx	,	ecx; сравнение b и 0
	jz	error; ошибка деление на ноль
	mov	eax	,	ebx; <eax>=b
	imul 	ecx; <edx:eax> = b*b
	add	eax	,	-5; добавление в младшую часть
	; <eax> = b*b Ц 5
	adc	edx	,	-1; коррекци€ старшей части
	; <edx:eax> = b*b Ц 5
	; обработка ситуации Ђчастное великої
	idiv	ecx; < eax> = (b*b Ц 5)/a
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
		cmp	ecx, ebx		; сравнение a и b
		jg l_bigger			; переход, если a > b
		jl l_smaller		; переход, если a < b
		mov	eax, 6			; <eax> = 6
		jmp	ex				; переход на конец программы

		l_bigger :
			or ecx, ecx		; сравнение a и 0
			jz errorZ		; ошибка делени€ на ноль
			mov	eax, ecx	; <eax> = a
			imul ecx		; <edx:eax> = a * a
			jo errorOF		; ошибка переполнени€
			sub eax, ebx	; <eax> = a * a - b
			jo errorOF		; ошибка переполнени€
			cdq				; <edx:eax> = a * a - b
			idiv ecx		; <eax> = (a * a - b) / a

			add eax, -3		; <eax> = (a * a - b) / a - 3
			jo errorOF		; ошибка переполнени€

			jmp	ex			; переход на конец программы
		
		l_smaller :
			or ebx, ebx		; сравнение b и 0
			jz errorZ		; ошибка делени€ на ноль
			
			mov	eax, ecx	; <eax> = a
			imul ecx		; <edx:eax> = a * a
			jo errorOF		; ошибка переполнени€
			add eax, -3		; <eax> = a * a - 3
			jo errorOF		; ошибка переполнени€
			cdq				; <edx:eax> = a * a - 3
			idiv ebx		; <eax> = (a * a - 3) / b

			jmp ex			; переход на конец программы

		; ошибка делени€ на ноль - обработка
		errorZ :
			mov err, 1
			jmp fin

		; ошибка переполнени€ - обработка
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
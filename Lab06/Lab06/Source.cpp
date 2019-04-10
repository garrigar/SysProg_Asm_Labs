#include <iostream>
#include <string>

using namespace std;

string calc(double a, double b) {
	const double c_3 = 3;
	const double c_6 = 6;
	double ans = 0;
	int err = 0;
	_asm {
		finit
		fld qword ptr[b]	; st(0) = b
		fld qword ptr[a]	; st(0) = a, st(1) = b
		fcom				; сравнение a и b
		fstsw ax			; сохраняем регистр флагов сопроцессора
		sahf				; записываем в регистр флагов процессора

		ja a_bigger			; переход если a больше
		jb b_bigger			; переход если b больше
							; если равны
		fld qword ptr[c_6]	; st(0) = 6, st(1) = a, st(2) = b
		jmp endcalc
		
		a_bigger :
		ftst				; st(0) = a, st(1) = b; сравнение a с 0
		fstsw ax			; сохраняем регистр флагов сопроцессора
		sahf				; записываем в регистр флагов процессора
		je err_0			; переход если a = 0
		fdiv st(1), st		; st(0) = a, st(1) = b / a
		fsubp st(1), st		; st(0) = (b / a) - a
		fadd qword ptr[c_3]	; st(0) = (b / a) - a + 3
		fchs				; st(0) = a - (b / a) - 3
		jmp endcalc

		b_bigger:
		fldz				; st(0) = 0, st(1) = a, st(2) = b
		fcomp st(2)			; st(0) = a, st(1) = b; сравнение b с 0
		fstsw ax			; сохраняем регистр флагов сопроцессора
		sahf				; записываем в регистр флагов процессора
		je err_0			; переход если b = 0
		fmul qword ptr[a]	; st(0) = a * a, st(1) = b
		fsub qword ptr[c_3]	; st(0) = a * a - 3, st(1) = b
		fxch st(1)			; st(0) = b, st(1) = a * a - 3
		fdivp st(1), st		; st(0) = (a * a - 3) / b
		jmp endcalc

		err_0:
		mov	err, 1
		jmp ex

		endcalc:
		fst	qword ptr[ans]; ans = st(0)

		ex:
	}
	if (err)
		return "Division by 0";
	return to_string(ans);
}

int main() {
	double a, b;
	cout << "a = ";
	cin >> a;
	cout << "b = ";
	cin >> b;
	cout << "result: " << calc(a, b) << endl;
	system("pause");
}
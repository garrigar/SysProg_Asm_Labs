#include <iostream>
#include <string>

using namespace std;

string calc_cpp(double a, double c, double d) {
	double denominator = a / 4 - 1;
	if (denominator == 0)
		return "Denominator is 0";
	return to_string((-2 * c + d * 82) / denominator);
}

string calc_asm(double a, double c, double d) {
	const double c_n2 = -2;
	const double c_82 = 82;
	const double c_4 = 4;
	const double c_1 = 1;
	double ans = 0;
	int err = 0;
	_asm {
		finit

		fld		qword ptr[c]	; st(0) = c
		fmul	qword ptr[c_n2]	; st(0) = c * (-2)
		fld		qword ptr[d]	; st(0) = d, st(1) = c * (-2)
		fmul	qword ptr[c_82]	; st(0) = d * 82, st(1) = c * (-2)
		faddp	st(1), st		; st(0) = c * (-2) + d * 82

		fld		qword ptr[a]	; st(0) = a, st(1) = c * (-2) + d * 82
		fdiv	qword ptr[c_4]	; st(0) = a / 4, st(1) = c * (-2) + d * 82
		fsub	qword ptr[c_1]	; st(0) = a / 4 - 1, st(1) = c * (-2) + d * 82

		fldz					; st(0) = 0, st(1) = a / 4 - 1, st(2) = c * (-2) + d * 82
		fcomp					; st(0) = a / 4 - 1, st(1) = c * (-2) + d * 82
		fstsw	ax
		sahf
		je		err_zerodiv		; jump if (a / 4 - 1 == 0)

		fdivp	st(1), st		; st(0) = (c * (-2) + d * 82) / (a / 4 - 1)
		;fxam
		fst		qword ptr[ans]	; ans = (c * (-2) + d * 82) / (a / 4 - 1)
		jmp		ex

		err_zerodiv:
		mov		err, 1

		ex:
	}
	if (err) 
		return "Denominator is 0";
	return to_string(ans);
}

int main() {
	double a, c, d;
	cout << "a = ";
	cin >> a;
	cout << "c = ";
	cin >> c; 
	cout << "d = ";
	cin >> d;
	cout << "asm: " << calc_asm(a, c, d) << endl << "cpp: " << calc_cpp(a, c, d) << endl;
	system("pause");
}

#include <iostream>
#include <iomanip>

using namespace std;

// ������� ������� �������� ln(x+1) ����������� � ��� (-1 < x <= 1)
// x - ��������, eps - ��������� ��������, analytic - ������������� �������� �������
// ���������� � terms_num ���-�� ������ ����, �������������� ��� ���������� ������ ��������, � actual_eps - ����������� ��������
double ln_xp1_series(double x, double eps, double analytic, int& terms_num, double& actual_eps) {

	double ans = 0;		// ������� �����
	int err = 0;		// ���� ������
	int t_num = 0;		// ���������� ������ ����
	double act_eps = 0; // ����������� �����������

	double n_x;			// ��� ����� ������� x*(-1)

	_asm {
		finit;

		fld x;			// st(0) = x
		fld1;			// st(0) = 1, st(1) = x
		fcomp;			// st(0) = x, ��������� 1 � x
		fstsw ax;		// ��������� ������� ������ ������������
		sahf;			// ���������� � ������� ������ ����������
		je cont1;		// ������� ���� 1 == x

		; // ���� x != 1
		fabs;			// st(0) = abs(x)
		fld1;			// st(0) = 1, st(1) = abs(x)
		fcomp;			// st(0) = abs(x), ��������� 1 � abs(x)
		fstsw ax;		// ��������� ������� ������ ������������
		sahf;			// ���������� � ������� ������ ����������
		ja cont1;		// ������� ���� 1 > abs(x)

		; // ���� abs(x) >= 1 � x != 1 - ��� ����������
		mov err, 1;		// ������������ ������
		jmp ex;			// �� �����

	cont1:
		; // ��� ��������
		
		ffree st(0);	// st - empty
		fld x;			// st(0) = x
		fchs;			// st(0) = -x
		fstp n_x;		// st - empty, n_x = -x

		fld1;			// st(0) = 1
		fchs;			// st(0) = -1 { = ((-1)^n * x^(n+1)) ��� n = -1 }

		xor ebx, ebx;	// ebx = 0 { = (n + 1) ��� n = -1 }
		
		; // ��������� "����" ���������� � n = 0
	lp:

		fmul n_x;		// st(0) = ((-1)^n * x^(n+1)) = (-x)^n * x

		inc ebx;		// ebx = n + 1
		mov t_num, ebx; // t_num = ebx = n + 1

		fld st(0);		// st(0) = st(1) = (-x)^n * x
		fidiv t_num;	// st(0) = ((-x)^n * x) / (n + 1), st(1) = (-x)^n * x

		fadd ans;		// st(0) = ans + ((-x)^n * x) / (n + 1), st(1) = (-x)^n * x
		fst ans;		// ans_new = st(0) = ans + ((-x)^n * x) / (n + 1), st(1) = (-x)^n * x

		fsub analytic;	// st(0) = ans_new - analytic, st(1) = (-x)^n * x
		fabs;			// st(0) = abs( ans_new - analytic ), st(1) = (-x)^n * x
		fst act_eps;	// act_eps = st(0) = abs( ans_new - analytic ), st(1) = (-x)^n * x

		fld eps;		// st(0) = eps, st(1) = act_eps, st(2) = (-x)^n * x
		fcompp;			// st(0) = (-x)^n * x, ��������� eps � act_eps
		fstsw ax;		// ��������� ������� ������ ������������
		sahf;			// ���������� � ������� ������ ����������
		jb lp;			// �������, ���� eps < act_eps, �.�. ���� �������� ������������

	ex :
	}
	// ���� ��� ����������, �������� ���������� NaN
	if (err) {
		return std::numeric_limits<double>::quiet_NaN();
	}
	terms_num = t_num;
	actual_eps = act_eps;
	return ans;
}

int main() {

	double x, eps;
	cout << "Input x and eps:" << endl;
	cout << "x = ";
	cin >> x;
	cout << "eps = ";
	cin >> eps;

	// ���: x > -1
	if (x <= -1) {
		cout << "x is out of domain of definition" << endl;
	}
	else {
		double analytic = log(x + 1);
		cout << "Analytic value: " << setprecision(10) << analytic << endl;
		
		int terms_num = 0;
		double act_eps = 0;
		double ans = ln_xp1_series(x, eps, analytic, terms_num, act_eps);
		// ���� ���������� �������� - NaN, ��� ����������
		if (isnan(ans)) {
			cout << "Series diverges" << endl;
		}
		else {
			cout << "Series value: " << setprecision(10) << ans << endl;
			cout << "Number of terms: " << terms_num << endl;
			cout << "Reached accuracy: " << act_eps << endl;
		}
	}
	system("pause");
	return 0;
}
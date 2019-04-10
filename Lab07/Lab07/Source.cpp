#include <iostream>
#include <iomanip>

using namespace std;

// Функция считает значение ln(x+1) разложением в ряд (-1 < x <= 1)
// x - аргумент, eps - требуемая точность, analytic - аналитическое значение функции
// Записывает в terms_num кол-во членов ряда, понадобившееся для достижения нужной точности, в actual_eps - достигнутую точность
double ln_xp1_series(double x, double eps, double analytic, int& terms_num, double& actual_eps) {

	double ans = 0;		// будущий ответ
	int err = 0;		// флаг ошибки
	int t_num = 0;		// количество членов ряда
	double act_eps = 0; // достигнутая погрешность

	double n_x;			// тут будем хранить x*(-1)

	_asm {
		finit;

		fld x;			// st(0) = x
		fld1;			// st(0) = 1, st(1) = x
		fcomp;			// st(0) = x, сравнение 1 и x
		fstsw ax;		// сохраняем регистр флагов сопроцессора
		sahf;			// записываем в регистр флагов процессора
		je cont1;		// переход если 1 == x

		; // если x != 1
		fabs;			// st(0) = abs(x)
		fld1;			// st(0) = 1, st(1) = abs(x)
		fcomp;			// st(0) = abs(x), сравнение 1 и abs(x)
		fstsw ax;		// сохраняем регистр флагов сопроцессора
		sahf;			// записываем в регистр флагов процессора
		ja cont1;		// переход если 1 > abs(x)

		; // если abs(x) >= 1 и x != 1 - ряд расходится
		mov err, 1;		// регистрируем ошибку
		jmp ex;			// на выход

	cont1:
		; // ряд сходится
		
		ffree st(0);	// st - empty
		fld x;			// st(0) = x
		fchs;			// st(0) = -x
		fstp n_x;		// st - empty, n_x = -x

		fld1;			// st(0) = 1
		fchs;			// st(0) = -1 { = ((-1)^n * x^(n+1)) при n = -1 }

		xor ebx, ebx;	// ebx = 0 { = (n + 1) при n = -1 }
		
		; // следующий "цикл" начинается с n = 0
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
		fcompp;			// st(0) = (-x)^n * x, сравнение eps и act_eps
		fstsw ax;		// сохраняем регистр флагов сопроцессора
		sahf;			// записываем в регистр флагов процессора
		jb lp;			// переход, если eps < act_eps, т.е. если точность недостаточна

	ex :
	}
	// если ряд расходится, присвоим результату NaN
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

	// ОДЗ: x > -1
	if (x <= -1) {
		cout << "x is out of domain of definition" << endl;
	}
	else {
		double analytic = log(x + 1);
		cout << "Analytic value: " << setprecision(10) << analytic << endl;
		
		int terms_num = 0;
		double act_eps = 0;
		double ans = ln_xp1_series(x, eps, analytic, terms_num, act_eps);
		// если полученное значение - NaN, ряд расходится
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
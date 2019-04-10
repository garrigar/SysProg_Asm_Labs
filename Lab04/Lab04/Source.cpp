#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string calc_mas(int* mas, int siz, int b, int d) {
	int result = 0;
	int err = 0;
	_asm {
		xor esi, esi	; обнуляем регистр индекса в массиве
		xor edi, edi	; обнуляем искомую сумму

		mov ebx, mas	; ebx указывает на начало массива
		mov ecx, siz	; счётчик цикла := количество элементов массива
		
		;;;;; jеcxz ex	; завершить, если длина массива 0
		
		or ecx, ecx		; хотим сравнить ecx с 0
		jz ex			; если 0, то уходим на вывод ответа
		jns begin_loop	; если не 0 и не отрицательное, начинаем цикл

		mov err, 1		; фиксируем ошибку в длине массива
		jmp fin			; выход из функции
		
		begin_loop:
			mov eax, [ebx + ecx * 4 - 4]	; определяем текущий элемент
			mov edx, d					; подготовка сравнения с d
			cmp eax, edx				; сравнение a[i] и d
			jg end_loop					; если больше, то завершаем цикл
			mov edx, b					; подготовка сравнения с b
			cmp eax, edx				; сравнение a[i] и b
			jl end_loop					; если меньше, то завершаем цикл
			
			add edi, eax				; элемент удовлетворяет условию, прибавляем его к сумме
			jno end_loop				; если нет переполнения, идем дальше

			mov err, 2					; фиксируем ошибку переполнения
			jmp fin						; выходим из функции

		end_loop :
			inc esi						; переходим к следующему элементу
			loop begin_loop				; повторяем цикл для всех элементов массива
		ex : 
			mov eax, edi				; возвращаем сумму элементов
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

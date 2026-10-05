#include <iostream>
#include <stdio.h>

using namespace std;

int ackermann_recursive(int m, int n) {
	
	if (m == 0) return n += 1;
	else if (n == 0) return ackermann_recursive(m - 1, 1);
	else return ackermann_recursive(m - 1, ackermann_recursive(m, n - 1));
}

void push(int*& s, int &capacity, int &top, int m) {
	if (top + 1 >= capacity) {
		int new_capacity = capacity * 2;
		int* new_s = new int[new_capacity];
		for (int i = 0; i <= top; ++i) {
			new_s[i] = s[i];
		}
		delete[] s;
		s = new_s;
		capacity = new_capacity;
	}

	top += 1;
	s[top] = m;
}

int pop(int* s, int& top) {
	int num = s[top];
	top--;
	return num;
}

int nonrecursive_ackermann(int m, int n) {
	int capacity = 16;
	int top = -1;
	int* s = new int[capacity];
	push(s, capacity, top, m);
	while (top >= 0) {
		m = pop(s, top);
		if (m == 0) {
			n += 1;
		}
		else if (n == 0) {
			n = 1;
			push(s, capacity, top, m - 1);
		}
		else {
			n -= 1;
			push(s, capacity, top, m - 1);
			push(s, capacity, top, m);
		}
	}
	delete[] s;
	return n;
}

int main() {
	int n, m;
	cout << "請輸入m, n的值：";
	while (cin >> m >> n) {
		if (m < 0 || n < 0) cout << "請輸入大於等於0的數。" << endl;
		else cout << endl << "遞迴函式：" << ackermann_recursive(m, n) << endl << "非遞迴函式(堆疊)：" << nonrecursive_ackermann(m, n) << endl;
		cout << endl <<"請輸入m, n的值：";
	}
	return 0;
}

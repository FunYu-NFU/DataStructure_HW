#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

class Polynomial;

class Term {
	friend class Polynomial;
	friend istream& operator>>(istream& s, Polynomial& p);
	friend ostream& operator<<(ostream& s, Polynomial& p);
private:
	float coef;
	int exp;
public:
	Term() {};
	Term(float ccoef, int eexp) {
		coef = ccoef; exp = eexp;
	}
};

class Polynomial {
private:
	Term* arrayTerm;
	int capacity; int terms;
public:
	Polynomial() {
		capacity = 2;
		terms = 0;
		arrayTerm = new Term[capacity];
	}
	~Polynomial() { delete[] arrayTerm; }
	Polynomial(const Polynomial& poly) { //*******複製建構子
		this->terms = poly.terms;
		this->capacity = poly.capacity;
		this->arrayTerm = new Term[poly.capacity];
		for (int i = 0; i < poly.terms; i++) {
			this->arrayTerm[i] = poly.arrayTerm[i];
		}
	}

	Polynomial& operator=(const Polynomial& poly) { //*******複製賦值建構子
		if (this == &poly) { return *this; }
		delete[] this->arrayTerm;
		this->terms = poly.terms;
		this->capacity = poly.capacity;
		this->arrayTerm = new Term[capacity];
		for (int i = 0; i < poly.terms; i++) {
			this->arrayTerm[i] = poly.arrayTerm[i];
		}
		return *this;
	}

	void addTerm(const float Incoef, const int Inexp) { //********新增多項式
		if (Incoef == 0) return;
		if (this->terms == this->capacity) {
			this->capacity *= 2;
			Term* temp = new Term[capacity];
			copy(this->arrayTerm, this->arrayTerm + terms, temp);
			delete[] this->arrayTerm;
			this->arrayTerm = temp;
		}
		this->arrayTerm[terms].coef = Incoef;
		this->arrayTerm[terms++].exp = Inexp;
	}

	void P_sort() { //********排序多項式(指數大到小)
		for (int i = 0; i < this->terms; i++) {
			for (int j = i + 1; j < this->terms; j++) {
				if (arrayTerm[i].exp < arrayTerm[j].exp) {
					swap(arrayTerm[i], arrayTerm[j]);
				}
			}
		}
	}

	void reset() { //********重置多項式陣列
		delete[] arrayTerm;
		capacity = 2;
		terms = 0;
		arrayTerm = new Term[capacity];
	}

	Polynomial add(const Polynomial& poly) { //********兩個多項式相加
		Polynomial c;
		int a_pos = 0; int b_pos = 0;
		while (a_pos < this->terms && b_pos < poly.terms) {
			if (this->arrayTerm[a_pos].exp == poly.arrayTerm[b_pos].exp) {
				float t = this->arrayTerm[a_pos].coef + poly.arrayTerm[b_pos].coef;
				if (t) c.addTerm(t, arrayTerm[a_pos].exp);
				a_pos++; b_pos++;
			}
			else if (this->arrayTerm[a_pos].exp < poly.arrayTerm[b_pos].exp) {
				c.addTerm(poly.arrayTerm[b_pos].coef, poly.arrayTerm[b_pos].exp);
				b_pos++;
			}
			else {
				c.addTerm(this->arrayTerm[a_pos].coef, this->arrayTerm[a_pos].exp);
				a_pos++;
			}
		}
		for (; a_pos < this->terms; a_pos++) c.addTerm(this->arrayTerm[a_pos].coef, this->arrayTerm[a_pos].exp);
		for (; b_pos < poly.terms; b_pos++) c.addTerm(poly.arrayTerm[b_pos].coef, poly.arrayTerm[b_pos].exp);
		return c;
	}

	Polynomial mult(const Polynomial& poly) { //********兩個多項式相乘
		Polynomial c;
		for (int a_pos = 0; a_pos < this->terms; a_pos++) {
			Polynomial temp;
			for (int b_pos = 0; b_pos < poly.terms; b_pos++) {
				float ccoef = this->arrayTerm[a_pos].coef * poly.arrayTerm[b_pos].coef;
				int eexp = this->arrayTerm[a_pos].exp + poly.arrayTerm[b_pos].exp;
				temp.addTerm(ccoef, eexp);
			}
			c = c.add(temp);
		}
		return c;
	}

	float Eval(float f) { //********代入x = f計算方程式值
		float sum = 0;
		for (int i = 0; i < this->terms; i++) {
			sum += this->arrayTerm[i].coef * pow(f, this->arrayTerm[i].exp);
		}
		return sum;
	}
	friend istream& operator>>(istream& s, Polynomial& p);
	friend ostream& operator<<(ostream& s, Polynomial& p);
};

istream& operator>>(istream& s, Polynomial& p) { //********輸入串流運算子多載
	int n;
	s >> n;
	while (n--) {
		float coef; int exp;
		s >> coef >> exp;
		p.addTerm(coef, exp);
	}
	return s;
}

ostream& operator<<(ostream& s, Polynomial& p) { //********//輸出串流運算子多載
	if (p.terms == 0) {
		cout << 0 << endl;
		return s;
	}
	else {
		p.P_sort();
		for (int i = 0; i < p.terms; i++) {
			float ccoef = p.arrayTerm[i].coef;
			int eexp = p.arrayTerm[i].exp;
			if (ccoef != 1 && eexp != 1) {
				if (ccoef > 0 && i == 0) s << ccoef << "X^" << eexp;
				else if (ccoef > 0 && i > 0 && i < (p.terms - 1)) s << " + " << ccoef << "X^" << eexp;
				else if (ccoef > 0 && i == p.terms - 1) s << " + " << ccoef << "X^" << eexp;
				else if (ccoef < 0 && i < 1) s << "-" << abs(ccoef) << "X^" << eexp;
				else if (ccoef < 0 && i > 1) s << " - " << abs(ccoef) << "X^" << eexp;
			}
			else if (ccoef == 1 && eexp != 1) {
				if (ccoef > 0 && i == 0) s << "X^" << eexp;
				else if (ccoef > 0 && i > 0 && i < (p.terms - 1)) s << " + X^" << eexp;
				else if (ccoef > 0 && i == p.terms - 1) s << " + " << "X^" << eexp;
				else if (ccoef < 0 && i < 1) s << "-X^" << eexp;
				else if (ccoef < 0 && i > 1) s << " - X^" << eexp;
			}
			else if (eexp == 1 && ccoef == 1) {
				if (ccoef > 0 && i == 0) s << "X";
				else if (ccoef > 0 && i > 0 && i < (p.terms - 1)) s << " + X";
				else if (ccoef > 0 && i == p.terms - 1) s << " + " << "X";
				else if (ccoef < 0 && i < 1) s << "-X";
				else if (ccoef < 0 && i > 1) s << " - X";
			}
			else if (eexp == 1 && ccoef != 1) {
				if (ccoef > 0 && i == 0) s << ccoef << "X";
				else if (ccoef > 0 && i > 0 && i < (p.terms - 1)) s << " + " << ccoef << "X";
				else if (ccoef > 0 && i == p.terms - 1) s << " + " << ccoef << "X";
				else if (ccoef < 0 && i < 1) s << "-" << abs(ccoef) << "X";
				else if (ccoef < 0 && i > 1) s << " - " << abs(ccoef) << "X";
			}
			else if (eexp == 0) {
				if (ccoef > 0 && i == 0) s << ccoef;
				else if (ccoef > 0 && i > 0 && i < (p.terms - 1)) s << " + " << ccoef;
				else if (ccoef > 0 && i == p.terms - 1) s << " + " << ccoef;
				else if (ccoef < 0 && i < 1) s << "-" << abs(ccoef);
				else if (ccoef < 0 && i > 1) s << " - " << abs(ccoef);
			}
		}
		cout << endl;
	}
	return s;
}

int main() {
	cout << "請輸入多項式 A：";
	Polynomial a;
	cin >> a;
	cout << "請輸入多項式 B：";
	Polynomial b;
	cin >> b;
	while (1) {
		cout << endl << "========== Polynomial 選單 ========" << endl << "1. 顯示 A、B" << endl << "2. A + B" << endl << "3. A * B" << endl << "4. 計算 A(x)" << endl
			<< "5. 計算 B(x)" << endl << "6. 重新輸入 A" << endl << "7. 重新輸入 B" << endl << "0. 結束程式" << endl << "請選擇：";
		int n;
		cin >> n;
		switch (n) {
		case 1: {
			cout << endl << "A(x) = " << a;
			cout << endl << "B(x) = " << b;
			break;
		}
		case 2: {
			Polynomial added;
			cout << endl << "A(x) = " << a;
			cout << endl << "B(x) = " << b;
			added = a.add(b);
			cout << endl << "A(x) + B(x) = " << added;
			break;
		}
		case 3: {
			Polynomial multiplied;
			cout << endl << "A(x) = " << a;
			cout << "B(x) = " << b;
			multiplied = a.mult(b);
			cout << "A(x) * B(x) = " << multiplied;
			break;
		}
		case 4: {
			cout << endl << "請輸入x = ";
			float Ax; cin >> Ax;
			float Asum = a.Eval(Ax);
			cout << "A(x) = " << a;
			cout << "A(" << Ax << ") = " << Asum << endl;
			break;
		}
		case 5: {
			cout << endl << "請輸入x = ";
			float Bx; cin >> Bx;
			float Bsum = b.Eval(Bx);
			cout << "B(x) = " << b;
			cout << "B(" << Bx << ") = " << Bsum << endl;
			break;
		}
		case 6: {
			cout << endl << "舊的 A(x) = " << a;
			cout << "請重新輸入多項式 A：";
			a.reset();
			cin >> a;
			cout << "新的 A(x) = " << a;
			break;
		}
		case 7: {
			cout << endl << "舊的 B(x) = " << b;
			cout << "請重新輸入多項式 B：";
			b.reset();
			cin >> b;
			cout << "新的 B(x) = " << b;
			break;
		}
		case 0: {
			return 0;
		}
		default: {
			cout << endl << "未經定義的操作，請重新輸入" << endl;
			break;
		}
		}
	}
	return 0;
}

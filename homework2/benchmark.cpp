#include <iostream>
#include <chrono>
#include <cmath>
#include <algorithm>

using namespace std;

// 引入你的 Term 與 Polynomial 類別 (或直接複製你寫好的 class)
class Term {
public:
    float coef; int exp;
    Term() : coef(0), exp(0) {}
    Term(float c, int e) : coef(c), exp(e) {}
};

class Polynomial {
public:
    Term* termArray; int capacity; int terms;
    Polynomial() { capacity = 2; terms = 0; termArray = new Term[capacity]; }
    ~Polynomial() { delete[] termArray; }
    Polynomial(const Polynomial& p) {
        terms = p.terms; capacity = p.capacity;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++) termArray[i] = p.termArray[i];
    }
    Polynomial& operator=(const Polynomial& p) {
        if (this == &p) return *this;
        delete[] termArray;
        terms = p.terms; capacity = p.capacity;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++) termArray[i] = p.termArray[i];
        return *this;
    }
    void addTerm(float c, int e) {
        if (c == 0) return;
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            copy(termArray, termArray + terms, temp);
            delete[] termArray;
            termArray = temp;
        }
        termArray[terms].coef = c;
        termArray[terms++].exp = e;
    }
    Polynomial Add(Polynomial poly) {
        Polynomial c;
        int a = 0, b = 0;
        while (a < terms && b < poly.terms) {
            if (termArray[a].exp == poly.termArray[b].exp) {
                float t = termArray[a].coef + poly.termArray[b].coef;
                if (t) c.addTerm(t, termArray[a].exp);
                a++; b++;
            } else if (termArray[a].exp < poly.termArray[b].exp) {
                c.addTerm(poly.termArray[b].coef, poly.termArray[b].exp);
                b++;
            } else {
                c.addTerm(termArray[a].coef, termArray[a].exp);
                a++;
            }
        }
        for (; a < terms; a++) c.addTerm(termArray[a].coef, termArray[a].exp);
        for (; b < poly.terms; b++) c.addTerm(poly.termArray[b].coef, poly.termArray[b].exp);
        return c;
    }
    Polynomial Mult(Polynomial poly) {
        Polynomial c;
        for (int a = 0; a < terms; a++) {
            Polynomial temp;
            for (int b = 0; b < poly.terms; b++) {
                temp.addTerm(termArray[a].coef * poly.termArray[b].coef,
                             termArray[a].exp + poly.termArray[b].exp);
            }
            c = c.Add(temp);
        }
        return c;
    }
};

int main() {
    cout << "===== 開始自動量測 =====" << endl;
    
    // 1. 加法效能量測 (測試 N = 100, 500, 1000, 2000)
    int add_sizes[] = {100, 500, 1000, 2000};
    cout << "\n[多項式加法 Add() 執行時間]" << endl;
    for (int n : add_sizes) {
        Polynomial p1, p2;
        for (int i = n; i >= 1; i--) {
            p1.addTerm(1.5, i * 2);
            p2.addTerm(2.5, i * 2 - 1);
        }
        auto start = chrono::high_resolution_clock::now();
        Polynomial res = p1.Add(p2);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, micro> elapsed = end - start;
        cout << "項數 N = " << n << " : " << elapsed.count() << " 微秒 (us)" << endl;
    }

    // 2. 乘法效能量測 (測試 N = 50, 100, 200, 400)
    int mult_sizes[] = {50, 100, 200, 400};
    cout << "\n[多項式乘法 Mult() 執行時間]" << endl;
    for (int n : mult_sizes) {
        Polynomial p1, p2;
        for (int i = n; i >= 1; i--) {
            p1.addTerm(1.0, i);
            p2.addTerm(1.0, i);
        }
        auto start = chrono::high_resolution_clock::now();
        Polynomial res = p1.Mult(p2);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed = end - start;
        cout << "項數 N = " << n << " : " << elapsed.count() << " 毫秒 (ms)" << endl;
    }
    return 0;
}

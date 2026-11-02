#include <iostream>
#include <chrono>
#include <cmath>
#include <algorithm>

using namespace std;

class Term {
public:
    float coef;
    int exp;
    Term() : coef(0), exp(0) {}
    Term(float c, int e) : coef(c), exp(e) {}
};

class Polynomial {
public:
    Term* termArray;
    int capacity;
    int terms;

    Polynomial() {
        capacity = 2;
        terms = 0;
        termArray = new Term[capacity];
    }
    ~Polynomial() {
        delete[] termArray;
    }
    Polynomial(const Polynomial& p) {
        terms = p.terms;
        capacity = p.capacity;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++) termArray[i] = p.termArray[i];
    }
    Polynomial& operator=(const Polynomial& p) {
        if (this == &p) return *this;
        delete[] termArray;
        terms = p.terms;
        capacity = p.capacity;
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
    Polynomial Add(const Polynomial& poly) {
        Polynomial c;
        int a = 0, b = 0;
        while (a < terms && b < poly.terms) {
            if (termArray[a].exp == poly.termArray[b].exp) {
                float t = termArray[a].coef + poly.termArray[b].coef;
                if (t != 0) c.addTerm(t, termArray[a].exp);
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
    Polynomial Mult(const Polynomial& poly) {
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
    cout << "========================================" << endl;
    cout << "   Polynomial Performance Benchmark     " << endl;
    cout << "========================================" << endl;

    // 1. Measure Add()：重複執行 1000 次取平均值
    int add_sizes[] = {100, 500, 1000, 2000};
    int repeat = 1000; // 重複 1000 次放大時間
    cout << "\n--- [1] Add() Execution Time (Average of 1000 runs) ---" << endl;
    for (int n : add_sizes) {
        Polynomial p1, p2;
        for (int i = n; i >= 1; i--) {
            p1.addTerm(1.5f, i * 2);
            p2.addTerm(2.5f, i * 2 - 1);
        }
        auto start = chrono::high_resolution_clock::now();
        for (int k = 0; k < repeat; k++) {
            Polynomial res = p1.Add(p2);
        }
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, micro> elapsed = end - start;
        cout << "N = " << n << " terms : " << (elapsed.count() / repeat) << " us" << endl;
    }

    // 2. Measure Mult()
    int mult_sizes[] = {50, 100, 200, 400};
    cout << "\n--- [2] Mult() Execution Time ---" << endl;
    for (int n : mult_sizes) {
        Polynomial p1, p2;
        for (int i = n; i >= 1; i--) {
            p1.addTerm(1.0f, i);
            p2.addTerm(1.0f, i);
        }
        auto start = chrono::high_resolution_clock::now();
        Polynomial res = p1.Mult(p2);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> elapsed = end - start;
        cout << "N = " << n << " terms : " << elapsed.count() << " ms" << endl;
    }

    cout << "\nBenchmark Finished Successfully!" << endl;
    return 0;
}

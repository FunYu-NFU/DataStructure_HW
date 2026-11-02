# 41443145

## 作業二

## 解題說明

本題要求實作一個包含多項式相加、相乘、帶入 *x* 計算值等功能的類別，並且實作輸入、輸出運算子多載來實現物件的建立與呈現。

### 解題策略

1. 設計`Term`類別，僅儲存非零項的係數、指數，多項式只保留實際存在的項數`terms`。
2. 使用者想輸入的項數在編譯時還未知，透過陣列容量擴大來確保程式執行不必直接占用大量記憶體空間，導致浪費。
3. 加法使用雙指標 `a_pos`, `b_pos`進行單次線性掃描:
   1. 指數相等: 兩者係數相加，若不為零，加入新項式，雙指標前進。
   2. *A*指數大於*B*: 放入*A*當前項，*A*指標前進。
   3. *A*指數小於*B*: 放入*B*當前項，*B*指標前進。
   4. 帶其中一方遍歷完畢後，將另一方殘餘的項次放入至新多項式。
4. 乘法使用分配律，外層迴圈走遍*A*每項，內層走遍*B*每項，兩項相乘後放入暫存陣列`temp`，接著呼叫加法函數累加到總結果中。

## 程式實作

以下為程式碼: 
```cpp
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
	p.P_sort();
	return s;
}

ostream& operator<<(ostream& s, Polynomial& p) { //********//輸出串流運算子多載
	if (p.terms == 0) {
		s << "0";
		return s;
	}
	for (int i = 0; i < p.terms; i++) {
		float c = p.arrayTerm[i].coef;
		int e = p.arrayTerm[i].exp;
		if (i == 0) {
			if (c < 0) s << "-";
		}
		else {
			if (c > 0) s << " + ";
			else s << " - ";
		}
		float absC = fabs(c);
		if (e == 0) {
			s << absC;
		}
		else if (e == 1) {
			if (absC != 1) s << absC;
			s << "X";
		}
		else {
			if (absC != 1) s << absC;
			s << "X^" << e;
		}
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
```
## 效能分析
1. 時間複雜度:
   * (1)建構子與解構子:
     * 預設建構子`Polynomial()`: $O(1)$
     * 解構子`~Polynomial()`: $O(1)$
     * 複製建構子`Polynomial(const Polynomial& poly)`: $O(n)$ (設傳入物件項數為*n*)
     * 複製賦值運算子 `operator=`: $O(n)$ (設傳入物件項數為*n*)
   * (2)資料操作函數:
     * 新增項式 `addTerm(float, int)`: $O(n)$ (使用`copy()`複製原有的*n*個項數)
     * 排序函數 `P_sort()`: $O(n^2)$
     * 重置函數 `reset()`: $O(1)$
   * (3)多項式代數與求值計算:
     * 多項式加法 `add(const Polynomial& poly)`: $O(m + n)$
     * 多項式乘法 `mult(const Polynomial& poly)`: $O(m^2 * n)$
     * 數值求值 `Eval(float f)`: $O(n)$
   * (4)運算子多載:
     * 輸入運算子 `operator>>`: $O(n^2)$ (*n*為輸入的項數)
     * 輸出運算子 `operator<<`: $O(n)$
   * (5)主程式選單操作:
     * 顯示 *A*、*B*: $O(m + n)$ (*m*, *n*為*A*, *B*的項數)
     * *A* + *B*: $O(m + n)$ (*m*, *n*為*A*, *B*的項數)
     * *A X B*: $O(m^2 * n)$
     * 計算 $A(x)$: $O(n)$
     * 計算 $B(x)$: $O(n)$
     * 重新輸入 *A*: $O(n^2)$
     * 重新輸入 *B*: $O(n^2)$
     * 結束程式: $O(1)$
2. 空間複雜度:
   * (1)建構子與解構子:
     * 預設建構子`Polynomial()`: $O(1)$
     * 解構子`~Polynomial()`: $O(1)$
     * 複製建構子`Polynomial(const Polynomial& poly)`: $O(n)$
     * 複製賦值運算子 `operator=`: $O(n)$
   * (2)資料操作函數:
     * 新增項式 `addTerm(float, int)`: $O(1)$
     * 排序函數 `P_sort()`: $O(1)$
     * 重置函數 `reset()`: $O(1)$
   * (3)多項式代數與求值計算:
     * 多項式加法 `add(const Polynomial& poly)`: $O(m + n)$
     * 多項式乘法 `mult(const Polynomial& poly)`: $O(m * n)$
     * 數值求值 `Eval(float f)`: $O(1)$
   * (4)運算子多載:
     * 輸入運算子 `operator>>`: $O(n)$
     * 輸出運算子 `operator<<`: $O(1)$
   * (5)主程式選單操作:
     * 顯示 *A*、*B*: $O(1)$
     * *A* + *B*: $O(m + n)$
     * *A X B*: $O(m * n)$
     * 計算 $A(x)$: $O(1)$
     * 計算 $B(x)$: $O(1)$
     * 重新輸入 *A*: $O(n)$
     * 重新輸入 *B*: $O(n)$
     * 結束程式: $O(1)$

* 整體程式時間複雜度: $O(n^3)$ (乘法函數)
* 整體程式空間複雜度: $O(m * n)$ (乘法函數)

## 測試與驗證

### 測試案例

| 測試案例 | 測試目的 | 輸入多項式A、B | 輸入操作 | 預期輸出 | 實際輸出
|----------|---------|---------------|---------|---------|--------|
| 測試一    | 同類項合併與部分抵銷     | $`A = `$ `3 3 2 -2 1 5 0` <br> $`B = `$ `3 1 2 2 1 -3 0`   | `2` ( $A(x) + B(x)$ ) |  `4X^2 + 2`  | `4X^2 + 2`
| 測試二 | 完全抵銷為零多項式 |  $`A = `$  `2 2 2 4 0` <br>  $`B = `$  `2 -2 2 -4 0` | `2` ( $A(x) + B(x)$ ) | `0` | `0` 
| 測試三 | 乘法展開與平方差驗證 | $`A = `$ `2 1 1 1 0` <br>  $`B = `$  `2 1 1 -1 0` | `3` ( $A(x) * B(x)$ ) | `X^2 - 1` | `X^2 - 1`
| 測試四 | 係數係數±1、常數與求值 | $`A = `$ `4 -1 3 1 2 -1 1 -5 0` <br> $`B = `$ `0` | `4 0`(計算 $A(0)$ ) <br> `4 2`(計算 $A(2)$ ) | `-5` <br> `-11` | `5` <br> `-11`
| 測試五 | 亂序輸入降冪驗證 | $`A = `$ `3 2 1 5 3 -4 0` <br> $`B = `$ `0` | `1` (顯示 $A、B$ ) | `5X^3 + 2X - 4` | `5X^3 + 2X - 4`

### 編譯與執行指令

```shell
$ g++ -std=c++11 -Wall src/hw2(Polynomial).cpp -o hw2
$ ./hw2

請輸入多項式 A：3  3 2  -2 1  5 0
請輸入多項式 B：3  1 2   2 1 -3 0

========== Polynomial 選單 ========
1. 顯示 A、B
2. A + B
3. A * B
4. 計算 A(x)
5. 計算 B(x)
6. 重新輸入 A
7. 重新輸入 B
0. 結束程式
請選擇：2

A(x) = 3X^2 - 2X + 5
B(x) = X^2 + 2X - 3
A(x) + B(x) = 4X^2 + 2

========== Polynomial 選單 ========
請選擇：4

請輸入x = 2
A(x) = 3X^2 - 2X + 5
A(2) = 13
```

## 效能量測

### 執行benchmark.cpp

```shell
g++ -O2 benchmark.cpp -o benchmark
./benchmark
```

### 執行結果

(1) 多項式加法 `Add()` 執行時間（重複 1000 次平均）
| 多項式項數 $N$ | 執行時間 (微秒 $\mu s$) | 換算為毫秒 (ms) |
|--|--|--|
| 100 項 | 2.265 $\mu s$ | 0.0022 ms |
| 500 項 | 5.546 $\mu s$ | 0.0055 ms |
| 1000 項 | 8.652 $\mu s$ | 0.0086 ms |
| 2000 項 | 38.587 $\mu s$ | 0.0385 ms |

#### (2) 多項式乘法 `Mult()` 執行時間
| 多項式項數 $N$ | 執行時間 (毫秒 ms) | 換算為微秒 ($\mu s$) |
|--|--|--|
| 50 項 | < 1 ms | - |
| 100 項 | < 1 ms | - |
| 200 項 | 1.141 ms | 1,141 $\mu s$ |
| 400 項 | 2.382 ms | 2,382 $\mu s$ |


## 心得討論

1. 以前寫C++程式時幾乎沒有過主動處理動態記憶體的深層複製經驗。在這次實作中，才初次體會到當類別內部包含動態配置陣列時，一定要實作三法則(解構、複製、複製賦值)，否則執行時一定會報錯。

2. 在進行多項式加法 `Add()` 量測時，第一次量測發現耗時均顯示為 $0 \mu s$ ，理解到現代CPU(本台筆電CPU基礎時脈約 3.2 – 3.55 GHz)處理數千項資料連一微秒都用不上，後透過重複執行 1,000 次取平均的方法，成功獲取能夠上 $\mu s$ 的耗時，也印證了加法 ($O(n)$) 與乘法 ($O(n^3)$) 算法的巨大效能差異。

3. 在實作輸出串流多載時，一開始使用一大長串`if-else`判斷來做格式化輸出，容易出現漏印或跳項的錯誤，除錯起來也讓人血壓飆高，後來將判斷拆解為項間加減->係數絕對值->顯不顯示X和次方三個步驟，既提升可讀性也更穩定。

4. 目前乘法函數透過呼叫相加函數累加，時間複雜度為 $O(m^2 * n)$ ，如果要處理上萬甚至更多的數據，就必須改用更有效率的排序法或算法。

# 41443145

作業一 problem 1: 阿克曼函數 遞迴與非遞迴

## 1. 解題說明

### 問題描述
本題要求使用遞迴與非遞迴兩種方法，實作阿克曼函數。  
數學定義如下：

$$
A(m, n) = 
\begin{cases} 
n + 1 & \text{if } m = 0 \\ 
A(m - 1, 1) & \text{if } m > 0 \text{ and } n = 0 \\ 
A(m - 1, A(m, n - 1)) & \text{if } m > 0 \text{ and } n > 0 
\end{cases}
$$

### 解題策略

1. 遞迴版本：
   * 依照數學定義的三個條件進行條件分支（`if-else`）。
   * 基準條件（Base Case）為 $m = 0$，回傳 $n + 1$。
   * 其他條件依序呼叫自身，最深層為巢狀呼叫 $A(m - 1, A(m, n - 1))$。

2. 非遞迴版本：
   * 資料結構： 使用自訂堆疊儲存 $m$ 的數值，並由變數 $n$ 充當回傳值/累積器，避免儲存整組 $(m, n)$ 結構以節省記憶體空間。
   * 運算順序控制： 根據阿克曼函數定義 $A(m-1, A(m, n-1))$，外層運算需等待內層結果。由於堆疊是先進後出，因此在展開時必須先 push $m-1$、再 push $m$，確保下一輪迴圈優先取出頂端的 $m$ 進行內層計算。
   * 動態擴容： 初始堆疊容量設為 16。當堆疊滿載時（`top + 1 >= capacity`），配置原容量 2 倍（$\times 2$）的新陣列，複製舊資料後釋放原記憶體，防止陣列越界。

3. 邊界與例外防護：
   * 在主迴圈接收輸入時，若 $m < 0$ 或 $n < 0$，立即提示錯誤訊息而不執行運算，防範非負定義域外的非法輸入導致無窮迴圈或記憶體溢位。

---

## 2. 程式實作

以下為C++程式碼：

```cpp
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
```

---

## 3. 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(A(m, n))$
   * 詳細分析： 阿克曼函數中所有的運算本質上都依賴基準條件 $m = 0$ 觸發的 `n += 1`。每一次拆解最終都會收斂至一個個單位的遞增操作。整體函式的呼叫次數（或 `while` 迴圈展開與 pop 的次數）與輸出結果 $A(m, n)$ 成正比，因此時間複雜度為 $O(A(m, n))$。
   * 例如當 $m = 1$ 時，時間為 $O(n)$；當 $m = 2$ 時，時間為 $O(2n)$；當 $m = 3$ 時，時間為 $O(2^n)$ 呈指數級增長。

2. 空間複雜度：空間複雜度為 $O(A(m, n))$
   * 遞迴版本： 取決於系統呼叫堆疊的最大深度。在展開內層呼叫時，堆疊最深處與運算結果同階，因此空間複雜度為 $O(A(m, n))$。
   * 非遞迴版本： 雖然避開了系統呼叫堆疊，但自訂動態陣列堆疊中暫存的待運算 $m$ 元素數量，在最深展開路徑上同樣為 $O(A(m, n))$。堆疊容量透過倍增擴充，保證每次擴容的均攤時間為 $O(1)$。

---

## 4. 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $(m, n)$ | 遞迴預期輸出 | 非遞迴預期輸出 | 實際輸出（遞迴 / 非遞迴） | 測試說明 |
|:---:|:---:|:---:|:---:|:---:|:---|
| 測試一 | $m = 0, n = 0$ | 1 | 1 | 1 / 1 | 測試雙 0 邊界條件 |
| 測試二 | $m = 1, n = 2$ | 4 | 4 | 4 / 4 | 線性加法測試: $A(1, 2) = 2 + 2 = 4$ |
| 測試三 | $m = 2, n = 2$ | 7 | 7 | 7 / 7 | 乘法測試: $A(2, 2) = 2(2) + 3 = 7$ |
| 測試四 | $m = 3, n = 2$ | 29 | 29 | 29 / 29 | 指數運算: $A(3, 2) = 2^{2+3} - 3 = 29$ |
| 測試五 | $m = 3, n = 3$ | 61 | 61 | 61 / 61 | 擴容測試: 堆疊深度突破初始 16 觸發擴容 |
| 測試六 | $m = 4, n = 0$ | 13 | 13 | 13 / 13 | $m=4$ 安全邊界測試 |
| 測試七 | $m = -1, n = 2$ | 無 | 無 | 提示輸入錯誤 | 防止負數進入算到天荒地老 |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o ackermann "hw1-1(ackermann).cpp"
$ ./ackermann
請輸入m, n的值：1 2

遞迴函式：4
非遞迴函式(堆疊)：4

請輸入m, n的值：3 3

遞迴函式：61
非遞迴函式(堆疊)：61

請輸入m, n的值：-1 2
請輸入大於等於0的數。

請輸入m, n的值：
```

### 結論
1. 程式在各種合法輸入下，遞迴與非遞迴函式的計算結果完全一致，驗證了演算法的正確性。
2. 測試案例涵蓋邊界值($m=0$, $n=0$)、高階指數測試($m=3$)以及非法負數攔截。
3. 在 $A(3, 3)$ 的測試中，非遞迴堆疊成功觸發了動態倍增（從容量 16 擴增），無記憶體洩漏與越界發生。

---

## 5. 申論及開發報告

### 選擇自訂堆疊與動態陣列的原因

本實作在非遞迴版本中採用自訂堆疊與動態擴容機制，主要基於以下設計考量：

1. 避免 Stack Overflow
   * 一般遞迴呼叫依賴作業系統的呼叫堆疊（Call Stack），其預設大小通常僅有 1MB 至 8MB。阿克曼函數展開深度極大，遞迴極易造成堆疊溢位崩潰。
   * 自訂堆疊使用堆積區記憶體（Heap，透過 `new` 配置），能使用的空間高達數 GB，使非遞迴版本具備承受更深層展開的能力。

2. 精簡狀態設計（變數 $n$ 兼任回傳值）
   * 傳統模擬遞迴往往需要將傳入參數與狀態（`struct Frame { int m; int n; int ret; }`）全部打包壓入堆疊。
   * 本演算法巧妙利用阿克曼函數的特性：只有 $m$ 需要排隊等待，運算完成的數值直接由變數 $n$ 承接。因此堆疊僅需儲存單一整數 $m$，大幅節省記憶體使用量與資料搬移成本。

3. 動態擴容策略
   * 為兼顧效能與記憶體開銷，初始容量設為較小的 16；當空間不足時採用 $\times 2$ 的策略重新分配記憶體。每次 Push 的平均時間複雜度依然維持在 $O(1)$，在保證不溢位的前提下最大化執行效率。

---

作業一 problem 2: 冪集合

## 1. 解題說明

### 問題描述
本題要求使用遞迴方法, 實作給定集合 $S$ 的冪集合 (Power Set) 求解器.  
若集合 $S$ 包含 $n$ 個不重複元素, 其冪集合 $\text{powerset}(S)$ 定義為由 $S$ 的所有可能子集合所組成的集合, 總計包含 $2^n$ 個子集合.  
例如, 若輸入集合為 $S = (a, b, c)$, 則其冪集合輸出為:

$$
\text{powerset}(S) = \{(), (a), (b), (c), (a, b), (a, c), (b, c), (a, b, c)\}
$$

### 解題策略

1. 集合預處理 (排序與去重):
   * 數學上集合具有「元素相異性」. 讀入字串後, 使用 `<algorithm>` 的 `sort` 先行排序, 並搭配 `unique` 與 `erase` 去除重複字元 (例如輸入 `aab` 會被標準化為 `ab`), 確保集合大小 $m$ 正確.

2. 二元決策樹遞迴展開:
   * 對於集合中的每一個元素, 都有「不選 (false)」與「選 (true)」兩種可能.
   * 遞迴函式由 `index = 0` 推進至 $m$, 每一層負責決定第 `index` 個元素的取捨. 透過布林陣列 `chosen` 記錄狀態, 深度優先搜尋 (DFS) 遍歷出全部 $2^m$ 種子集路徑.

3. Base Case 格式化與動態暫存:
   * 當 `index == m` (抵達最底層) 時, 代表單一子集的取捨已定案.
   * 掃描 `chosen` 陣列將挑選的字元組裝為格式化字串 (如 `"(a, b)"`), 存入動態配置的 `subsets` 字串陣列中, 暫不直接輸出.

4. 長度優先與字典序自訂排序 (Comparator):
   * 撰寫 `compare` 函式給予 `std::sort` 作為比較規則:
     * 第一順位 (比長度): 字串長度較短的排前面 (`a.length() < b.length()`), 確保由空集、單元素子集、雙元素子集依序排列.
     * 第二順位 (比字典序): 若字串長度相同, 則依 ASCII 字典序排序 (`a < b`), 確保如 `(a)` 排在 `(b)` 前面.
     * 
---

## 2. 程式實作

以下為C++程式碼:

```cpp
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

string* subsets;
int subset_count = 0;

bool compare(const string& a, const string& b) {
	if (a.length() != b.length()) { return a.length() < b.length(); } //如果a, b長度不同，比對誰比較短 
	return a < b; //依照字母順序排
}

void powerset(int index, int m, bool* chosen, char* p) {
	if (index == m) {
		string current = "(";
		bool first_element = true;
		for (int i = 0; i < m; i++) {
			if (chosen[i]) {
				if (!first_element) current += ", ";
				current += p[i];
				first_element = false;
			}
		}
		current += ")";
		subsets[subset_count++] = current;
		return;
	}
	chosen[index] = false;
	powerset(index + 1, m, chosen, p);

	chosen[index] = true;
	powerset(index + 1, m, chosen, p);
}

int main() {
	string s;
	cout << "請輸入集合： ";
	while (cin >> s) {
		sort(s.begin(), s.end());
		s.erase(unique(s.begin(), s.end()), s.end()); //刪減相同元素 (a, a, b) -> (a, b)
		int m = s.length();
		char* p = new char[m];
		for (int i = 0; i < m; i++) p[i] = s[i];
		bool* chosen = new bool[m];
		int total_subsets = 1 << m; //m個個數的集合所有子集合的數量為2^m, 所以配置2^m
		subsets = new string[total_subsets];
		subset_count = 0;
		powerset(0, m, chosen, p);
		sort(subsets, subsets + total_subsets, compare); //利用sort()比對子集合的先後順序
		cout << "powerset(S) = {";
		for (int i = 0; i < total_subsets; i++) {
			if (i > 0) cout << ", ";
			cout << subsets[i];
		}
		cout << "}" << endl;
		delete[] p;
		delete[] chosen;
		delete[] subsets;
		cout << "請輸入集合： ";
	}
	return 0;
}
```

---

## 3. 效能分析

設原始輸入去重後的集合大小為 $m$:

1. 時間複雜度：程式的時間複雜度為 $O(m \cdot 2^m)$
   * 預處理階段: 對輸入字串排序與去重, 時間複雜度為 $O(m \log m)$.
   * 遞迴生成階段: 二元決策樹的高度為 $m$, 總共有 $2^m$ 個葉節點 (Base Case). 每個葉節點需花費 $O(m)$ 的時間遍歷 `chosen` 陣列來拼接字串, 故遞迴總工時為 $O(m \cdot 2^m)$.
   * 字串排序階段: 共有 $N = 2^m$ 個子集合字串. 使用 `std::sort` 排序需要 $O(N \log N)$ 次字串比對, 每次比對的字串長度最長為 $O(m)$, 故排序時間為:
     $$O(m \cdot N \log N) = O(m \cdot 2^m \cdot \log(2^m)) = O(m^2 \cdot 2^m)$$
   * 整體時間複雜度: 主導項為排序與生成, 整體為 $O(m^2 \cdot 2^m)$. 若在元素量 $m$ 較小時, 運算效率極高, 均能在毫秒級內完成.

2. 空間複雜度：空間複雜度為 $O(m \cdot 2^m)$
   * 系統呼叫堆疊: 遞迴的最大深度為 $m$ 層 (由 `index = 0` 到 `m`), 呼叫堆疊空間為 $O(m)$.
   * 輔助記憶體配置 (Heap):
     * 字元陣列 `p` 與布林狀態陣列 `chosen` 各佔用 $O(m)$ 空間.
     * 子集合暫存陣列 `subsets` 需儲存 $2^m$ 個字串, 每個字串佔用空間與元素數量成正比 (平均長度為 $O(m)$), 故堆積區空間為 $O(m \cdot 2^m)$.
   * 整體空間複雜度: 由子集合儲存空間主導, 總空間複雜度為 $O(m \cdot 2^m)$.

---

## 4. 測試與驗證

### 測試案例

| 測試案例 | 輸入集合 | 預期輸出 | 實際輸出 | 測試重點說明 |
|:---:|:---:|:---|:---|:---|
| 測試一 | `a` | `{(), (a)}` | `{(), (a)}` | 單元素集合邊界測試 ($2^1 = 2$) |
| 測試二 | `ab` | `{(), (a), (b), (a, b)}` | `{(), (a), (b), (a, b)}` | 雙元素集合長度排序測試 ($2^2 = 4$) |
| 測試三 | `abc` | `{(), (a), (b), (c), (a, b), (a, c), (b, c), (a, b, c)}` | `{(), (a), (b), (c), (a, b), (a, c), (b, c), (a, b, c)}` | 題目標準範例驗證 ($2^3 = 8$) |
| 測試四 | `cba` | `{(), (a), (b), (c), (a, b), (a, c), (b, c), (a, b, c)}` | `{(), (a), (b), (c), (a, b), (a, c), (b, c), (a, b, c)}` | 亂序輸入排序測試 (驗證前置 `sort`) |
| 測試五 | `aab` | `{(), (a), (b), (a, b)}` | `{(), (a), (b), (a, b)}` | 重複字元去重測試 (驗證 `unique`) |
| 測試六 | `123` | `{(), (1), (2), (3), (1, 2), (1, 3), (2, 3), (1, 2, 3)}` | `{(), (1), (2), (3), (1, 2), (1, 3), (2, 3), (1, 2, 3)}` | 數字字元泛用性測試 |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o powerset "hw1-2(powerset).cpp"
$ ./powerset
請輸入集合： abc
powerset(S) = {(), (a), (b), (c), (a, b), (a, c), (b, c), (a, b, c)}
請輸入集合： cba
powerset(S) = {(), (a), (b), (c), (a, b), (a, c), (b, c), (a, b, c)}
請輸入集合： aab
powerset(S) = {(), (a), (b), (a, b)}
請輸入集合： 
```

### 結論
1. 程式滿足題目對冪集合之定義, 面對各類輸入皆能精準輸出 $2^m$ 個子集合.
2. 透過 `sort` 與 `unique` 的前置處理, 成功防範了重複元素造成的集合定義衝突.

---

## 5. 申論及開發報告

### 演算法選擇與實作架構探討

本題在實作過程中針對多個設計層面進行了評估與取捨:

1. 為何選擇二元決策樹作為遞迴核心？
   * 一個大小為 $m$ 的集合中, 任何子集合的產生都可以對應為每個元素的二元指示變數 (Indicator Variable): $0$ 代表不取, $1$ 代表選取.
   * 遞迴結構天然對應此二元樹模型: 函式在每一層分別執行 `chosen[index] = false` 與 `chosen[index] = true`, 兩者各遞迴呼叫下一層. 這種設計邏輯清晰、分支對稱, 且能確保不重不漏地走訪所有 $2^m$ 種狀態.

2. 自訂比較器 `compare` 與 `const string&` 的效能考量
   * 在字串比較函式 `compare(const string& a, const string& b)` 中:
     * 首先比對 `a.length() < b.length()`, 利用字串的字元數必隨元素量遞增的特性, 達成長度排序.
     * 當長度相等時, 直接利用 `a < b` 進行 ASCII 字典序比對.
   * 參數型態採用 `const string&` 常數參考, 避免了排序過程中頻繁進行字串拷貝的記憶體開銷, 兼具執行安全與極致效能.

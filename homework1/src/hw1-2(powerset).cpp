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
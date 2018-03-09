#include <iostream>
#include <vector>
#include <string>

using namespace std;

int n;
string a, b;


int convert(int begin, int end, char c) {


	vector<int> groups(1, 0);
	int start = 0;
	if (a[begin] == '+') {
		start = 1;
	}
	else {
		start = 0;
	}

	char cuc = a[begin];
	for (int i = begin; i < end; i++) {
		if (a[i] != cuc) {
			groups.push_back(1);
			cuc = a[i];
		}
		else {
			groups[groups.size() - 1]++;
		}
	}

	vector<int> stack;
	if (c == '+') {
		int i = 0;
		stack.push_back(groups[i]); i++;

		if (start == 0) {
			if (groups.size() == 1)
				return 0;
			stack.push_back(groups[i]); i++;
		}

		int cur_c = start;
		while (stack[stack.size() - 1] != end - begin) {
			int s = stack.size();
			if (s > 1) {
				if (stack[s - 1] > stack[s - 2]) {
					stack[s - 2] += stack[s - 1];
					stack.pop_back();

					if (s > 2) {
						stack[s - 3] += stack[s - 2];
						stack.pop_back();
					}
					continue;
				}
			}

			if (stack[stack.size() - 1] == end - begin) {
				return 1;
			}
			if (i < groups.size()) {
				stack.push_back(groups[i]); i++;
				if (stack[stack.size() - 2] > stack[stack.size() - 1]) {
					stack[stack.size() - 2] += stack[stack.size() - 1];
					stack.pop_back();

					if (i < groups.size()) {
						stack[stack.size() - 1] += groups[i]; i++;
					}
				}
				else {
					if (i < groups.size()) {
						stack.push_back(groups[i]); i++;
					}
					else {
						return 0;
					}
				}
			}
			else {
				return 0;
			}
		}
	}
	else {
		int i = 0;
		stack.push_back(groups[i]); i++;

		if (start == 1) {
			if (groups.size() == 1)
				return 0;
			stack.push_back(groups[i]); i++;
		}

		int cur_c = start;
		while (stack[stack.size() - 1] != end - begin) {
			int s = stack.size();
			if (s > 1) {
				if (stack[s - 1] > stack[s - 2]) {
					stack[s - 2] += stack[s - 1];
					stack.pop_back();

					if (s > 2) {
						stack[s - 3] += stack[s - 2];
						stack.pop_back();
					}
					continue;
				}
			}

			if (stack[stack.size() - 1] == end - begin) {
				return 1;
			}
			if (i < groups.size()) {
				stack.push_back(groups[i]); i++;
				if (stack[stack.size() - 2] > stack[stack.size() - 1]) {
					stack[stack.size() - 2] += stack[stack.size() - 1];
					stack.pop_back();

					if (i < groups.size()) {
						stack[stack.size() - 1] += groups[i]; i++;
					}
				}
				else {
					if (i < groups.size()) {
						stack.push_back(groups[i]); i++;
					}
					else {
						return 0;
					}
				}
			}
			else {
				return 0;
			}
		}
	}
	return 1;
}

void input() {
	cin >> n;
}

void calculate() {
	for (int it = 0; it < n; it++) {
		cin >> a >> b;

		int success = 1;
		char c;
		int i = 0;
		while (i < b.size()) {
			c = b[i];
			int begin = i;
			while (i < b.size() && b[i] == c) {
				i++;
			}
			int end = i;

			if (begin > 0) {
				if (a[begin - 1] != a[begin] && a[begin] != b[begin] && a[begin - 1] != b[begin - 1] || a[begin - 1] == a[begin]) {
					success = 0;
				}
			}
			if (end < a.size()) {
				if (a[end - 1] != a[end] && a[end] != b[end] && a[end - 1] != b[end - 1] || a[end - 1] == a[end]) {
					success = 0;
				}
			}

			if (convert(begin, end, c)) {
				;
			}
			else {
				success = 0;
			}
		}

		if (!success) {
			cout << "No\n";
		}
		else {
			cout << "Yes\n";
		}
	}
}

void output() {
	/*cout << "Continue...\n";
	fflush(stdout);
	while (true) {
		;
	}*/
}

int main() {
	input();
	calculate();
	output();
	return 0;
}
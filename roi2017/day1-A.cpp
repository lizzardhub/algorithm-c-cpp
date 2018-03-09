// exa.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

// Variables
int n;
vector<int> seq; // 0 - (, 1 - )

// Functions

void input() {
	cin >> n;
	seq.assign(n, 0);
}

void calculate() {
	int balance = 1;

	int i = 0;
	int j = i + 1;
	vector<int> stack;
	stack.push_back(i);
	while (i < n) {
		cout << "? " << i + 1 << " " << j + 1 << "\n";
		fflush(stdout);

		string ans;
		cin >> ans;
		if (ans == "Yes") {
			seq[i] = 0;
			seq[j] = 1;
			j++;
			stack.pop_back();
			if (!stack.empty()) {
				i = stack[stack.size() - 1];
			}
			else {
				i = j;
				j++;
				stack.push_back(i);
			}
		}
		else {
			stack.push_back(j);
			i = j;
			j++;
		}
	}
}

void output() {
	cout << "! ";
	for (int x : seq) {
		if (x == 0) {
			cout << "(";
		}
		else {
			cout << ")";
		}
	}
	cout << "\n";
	fflush(stdout);
	//system("pause");
}

int main()
{
	input();
	calculate();
	output();
    return 0;
}

